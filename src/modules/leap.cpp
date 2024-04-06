#include "leap.h"
#include <startup/vector_table.h>
#include <hc32_ddl.h>
#include "../config.h"
#include "log.h"
#include "flash.h"

namespace leap
{
  namespace checks
  {
    namespace impl
    {
      namespace helper
      {
        /**
         * @brief check if a pointer is null or erased
         * @param ptr the pointer to check
         * @return true if the pointer is null or erased
         */
        bool is_bogus_pointer(const void *ptr)
        {
          return ptr == nullptr || reinterpret_cast<uint32_t>(ptr) == 0xFFFFFFFF;
        }

        /**
         * @brief check if a address is in the area between APP_BASE_ADDRESS and the end of flash
         * @param address the address to check
         * @return true if the address is in the application area
         */
        bool is_in_app_area(const uint32_t address)
        {
          return address >= APP_BASE_ADDRESS && address < flash::get_flash_size();
        }

        /**
         * @brief check if a address is in the RAM area
         * @param address the address to check
         * @return true if the address is in the RAM area
         */
        bool is_in_ram(const uint32_t address)
        {
          return address >= 0x1FFF8000 && address <= 0x20026FFF;
        }

        /**
         * @brief check if the T bit is set in the address
         * @param address the address to check
         * @return true if the T bit is set
         */
        bool is_t_bit_set(const uint32_t address)
        {
          return (address & 0x1) == 1;
        }

        /**
         * @brief check if a interrupt vector seems valid:
         * - not null or bogus
         * - inside app flash area
         * - T bit set
         */
        bool is_valid_interrupt_vector(const irq_vector_t vector)
        {
          return !is_bogus_pointer(reinterpret_cast<void*>(vector)) &&
                 is_in_app_area(reinterpret_cast<uint32_t>(vector)) &&
                 is_t_bit_set(reinterpret_cast<uint32_t>(vector));
        }
      } // namespace helper

      /**
       * @brief check the stack pointer seems valid:
       * - not null or bogus
       * - inside RAM area
       */
      bool stack_pointer_seems_valid(const vector_table_t *app_vector_table)
      {
        return !helper::is_bogus_pointer(app_vector_table->stackTop) &&
               helper::is_in_ram(reinterpret_cast<uint32_t>(app_vector_table->stackTop));
      }

      /**
       * @brief check the reset handler seems valid
       */
      bool reset_handler_seems_valid(const vector_table_t *app_vector_table)
      {
        return helper::is_valid_interrupt_vector(app_vector_table->reset);
      }

      /**
       * @brief check the cpu vectors all seem valid
       * @note excludes reset handler, as it is checked separately
       */
      bool cpu_vectors_seem_valid(const vector_table_t *app_vector_table)
      {
        return helper::is_valid_interrupt_vector(app_vector_table->nmi) &&
               helper::is_valid_interrupt_vector(app_vector_table->hardFault) &&
               helper::is_valid_interrupt_vector(app_vector_table->memManageFault) &&
               helper::is_valid_interrupt_vector(app_vector_table->busFault) &&
               helper::is_valid_interrupt_vector(app_vector_table->usageFault) &&
               helper::is_valid_interrupt_vector(app_vector_table->svCall) &&
               helper::is_valid_interrupt_vector(app_vector_table->debugMonitor) &&
               helper::is_valid_interrupt_vector(app_vector_table->pendSV) &&
               helper::is_valid_interrupt_vector(app_vector_table->sysTick);
      }

      /**
       * @brief check reserved areas in the vector table are set to 0x0
       */
      bool reserved_vectors_seem_valid(const vector_table_t *app_vector_table)
      {
        return app_vector_table->reserved1[0] == 0x0 &&
               app_vector_table->reserved1[1] == 0x0 &&
               app_vector_table->reserved1[2] == 0x0 &&
               app_vector_table->reserved1[3] == 0x0 &&
               app_vector_table->reserved2 == 0x0;
      }

      /**
       * @brief check all NVIC interrupt vectors seem valid
       */
      bool nvic_vectors_seem_valid(const vector_table_t *app_vector_table)
      {
        for (const irq_vector_t &vector : app_vector_table->irqs)
        {
          if (!helper::is_valid_interrupt_vector(vector))
          {
            return false;
          }
        }

        return true;
      }
    } // namespace impl

    /**
     * @brief function pointer to run prelaunch checks on the application
     */
    typedef bool (*pre_check_function)(const vector_table_t *app_vector_table);

    /**
     * @brief list of prelaunch checks to run on the application
     */
    const pre_check_function pre_checks[] = {
      #if IS_PRE_CHECK_LEVEL(PRE_CHECK_MINIMAL)
        impl::stack_pointer_seems_valid,
        impl::reset_handler_seems_valid,
      #endif
      #if IS_PRE_CHECK_LEVEL(PRE_CHECK_EXTENDED)
        impl::cpu_vectors_seem_valid,
      #endif
      #if IS_PRE_CHECK_LEVEL(PRE_CHECK_FULL)
        impl::reserved_vectors_seem_valid,
        impl::nvic_vectors_seem_valid,
      #endif
    };
  }

  bool pre_check(const uint32_t app_base_address)
  {
    #if IS_PRE_CHECK_LEVEL(PRE_CHECK_MINIMAL)    
      const vector_table_t *app_vector_table = reinterpret_cast<vector_table_t *>(app_base_address);

      // run all pre-checks
      int i = 0;
      for (const auto &check : checks::pre_checks)
      {
        if (!check(app_vector_table))
        {
          logging::debug("pre-check #");
          logging::debug(i, 10);
          logging::debug(" failed\n");
          return false;
        }

        i++;
      }
    #endif

    // all checks passed
    return true;
  }

  void jump(const uint32_t app_base_address)
  {
    const vector_table_t *app_vector_table = reinterpret_cast<vector_table_t *>(app_base_address);

    // update stack pointer and VTOR
    __disable_irq();
    __set_MSP(reinterpret_cast<uint32_t>(app_vector_table->stackTop));
    SCB->VTOR = app_base_address;
    __DSB();
    __enable_irq();

    // jump to the application
    app_vector_table->reset();
  }
} // namespace leap

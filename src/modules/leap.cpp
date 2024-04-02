#include "leap.h"
#include <startup/startup.h>
#include <Arduino.h>

namespace leap
{
  bool jump(const uint32_t app_base_address)
  {
    const vector_table_t *app_vector_table = reinterpret_cast<vector_table_t *>(app_base_address);

    // check if the reset handler seems valid
    // - not null
    // - not in the bootloader area
    if (app_vector_table->reset == nullptr || reinterpret_cast<uint32_t>(app_vector_table->reset) < app_base_address)
    {
      return false;
    }

    // update stack pointer and VTOR
    noInterrupts();
    __set_MSP(reinterpret_cast<uint32_t>(app_vector_table->stackTop));
    SCB->VTOR = app_base_address;
    __DSB();
    interrupts();

    // jump to the application
    app_vector_table->reset();
    return true;
  }
} // namespace leap

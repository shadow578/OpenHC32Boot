#include "../config.h"
#include "pins_helper.h"
#include "serial_helper.h"
#include "common.h"
#include "modules/flash.h"

// APP_BASE_ADDRESS must align to the following:
// - next power of 2 after the vector table size (= 1024 bytes)
// - erase sector size of the flash memory (= normally 8K, configured in flash module)
static_assert((APP_BASE_ADDRESS & (1024 - 1)) == 0, "APP_BASE_ADDRESS must be aligned to 1024 bytes");
static_assert((APP_BASE_ADDRESS & (flash::erase_sector_size - 1)) == 0, "APP_BASE_ADDRESS must be aligned to the erase sector size");

// APP_BASE_ADDRESS must also be after the bootloader region
// including any offset for the bootloader region
static_assert(APP_BASE_ADDRESS >= (LD_FLASH_START + APP_BASE_ADDRESS), "APP_BASE_ADDRESS must be after the bootloader region");

// total flash size must be valid
static_assert(flash::total_size == (256 * 1024) || flash::total_size == (512 * 1024), "total flash size must be 256 or 512 KBytes");

// serial peripherals should not be duplicated
// only valid if both are enabled
#if HAS_SERIAL(HOST_SERIAL) && HAS_SERIAL(SCREEN_SERIAL)
  static_assert(HOST_SERIAL != SCREEN_SERIAL, "HOST_SERIAL and SCREEN_SERIAL must be different");
#endif

// serial peripherals should be valid
static_assert(is_valid_usart(HOST_SERIAL, true), "HOST_SERIAL must be a valid USART");
static_assert(is_valid_usart(SCREEN_SERIAL, true), "SCREEN_SERIAL must be a valid USART");

// host serial should use a valid and tested baud rate
constexpr int host_allowed_baud_rates[] = {
  9600, 19200, 38400, 57600, 115200
};
constexpr int host_baud = HOST_SERIAL_BAUD;
static_assert(contains(host_allowed_baud_rates, host_baud), "HOST_SERIAL_BAUD must be a valid baud rate");

// pins should not be reused
constexpr gpio::pin_t sdio_pins[] = SDIO_PINS;
constexpr gpio::pin_t loose_pins[] = {
  #if HAS_SERIAL(HOST_SERIAL)
    HOST_SERIAL_TX,
  #endif
  #if HAS_SERIAL(SCREEN_SERIAL)
    SCREEN_SERIAL_TX,
  #endif
  #if defined(BEEPER_PIN)
    BEEPER_PIN,
  #endif
};

static_assert(!has_duplicates(loose_pins), "Pins must not be duplicated");
static_assert(!has_duplicates(sdio_pins), "SDIO pins must not be duplicated");
static_assert(!containsAny(sdio_pins, loose_pins), "SDIO pins must not be reused");

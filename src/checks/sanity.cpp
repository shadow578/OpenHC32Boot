#include "../config.h"
#include "pins_helper.h"
#include "serial_helper.h"
#include "common.h"
#include "../modules.h"

// APP_BASE_ADDRESS must align to the following:
// - next power of 2 after the vector table size (= 1024 bytes)
// - erase sector size of the flash memory (= normally 8K, configured in flash module)
static_assert((APP_BASE_ADDRESS & (1024 - 1)) == 0, "APP_BASE_ADDRESS must be aligned to 1024 bytes");
static_assert((APP_BASE_ADDRESS & (flash::erase_sector_size - 1)) == 0, "APP_BASE_ADDRESS must be aligned to the erase sector size");

// APP_BASE_ADDRESS must also be after the bootloader region
// including any offset for the bootloader region
static_assert(APP_BASE_ADDRESS >= (LD_FLASH_START + APP_BASE_ADDRESS), "APP_BASE_ADDRESS must be after the bootloader region");

// serial peripherals should not be duplicated
// only valid if both are enabled
#if HAS_SERIAL(HOST_SERIAL) && HAS_SERIAL(SCREEN_SERIAL)
  static_assert(HOST_SERIAL != SCREEN_SERIAL, "HOST_SERIAL and SCREEN_SERIAL must be different");
#endif

// serial peripherals should be valid
#if HAS_SERIAL(HOST_SERIAL)
  static_assert(is_valid_usart(HOST_SERIAL, true), "HOST_SERIAL must be a valid USART");
#endif
#if HAS_SERIAL(SCREEN_SERIAL)
  static_assert(is_valid_usart(SCREEN_SERIAL, true), "SCREEN_SERIAL must be a valid USART");
#endif

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

// debug pins should not be used for something else
constexpr gpio::pin_t debug_pins[] = {
  gpio::PA13, // JTMS / SWDIO
  gpio::PA14, // JTCK / SWCLK
  gpio::PA15, // JTDI
  gpio::PB3,  // JTDO
  gpio::PB4,  // NJTRST
};

static_assert(!containsAny(debug_pins, sdio_pins), "Debug interface pins must not be used");
static_assert(!containsAny(debug_pins, loose_pins), "Debug interface pins must not be used");


// SDIO pins must be valid for either 1-bit, 4-bit or 8-bit bus width
static_assert(sdio::bus_width == 1 || sdio::bus_width == 4 || sdio::bus_width == 8, "SDIO_PINS must be valid for bus width of 1, 4 or 8 bits");

// pre-checks should not be disabled completely without annoying the user
static_assert(PRE_CHECK_LEVEL != PRE_CHECK_NONE, "PRE_CHECK_LEVEL should not be disabled completely");

// compat option to disable debug port should result in valid mask
#if COMPAT_DISABLE_DEBUG_PORT != 0
  static_assert((COMPAT_DISABLE_DEBUG_PORT & 0x1F) == COMPAT_DISABLE_DEBUG_PORT, "COMPAT_DISABLE_DEBUG_PORT must be a valid mask");
#endif

// if chipid variant override is enabled, there must be a override value
#if defined(CHIPID_VARIANT_OVERRIDE)
  #define HAS_CHIPID_VARIANT_OVERRIDE 1
#else
  #define HAS_CHIPID_VARIANT_OVERRIDE 0
#endif

static_assert(CHIPID_VARIANT_OVERRIDE_ENABLE == 0 || HAS_CHIPID_VARIANT_OVERRIDE == 1, "CHIPID_VARIANT_OVERRIDE must be defined if CHIPID_VARIANT_OVERRIDE_ENABLE is enabled");

#if defined(DELETE_FIRMWARE_UPDATE_FILE)
  #error "DELETE_FIRMWARE_UPDATE_FILE is no longer supported! Please remove it from the configuration."
#endif

#if defined(SKIP_FILE_CLEANUP)
  #error "SKIP_FILE_CLEANUP is no longer supported! Please remove it from the configuration."
#endif

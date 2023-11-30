#pragma once
#include "../config.h"
#if ENABLE_FLASH_PROTECTION == 1

/**
 * @brief enable flash protection for bootloader region
 */
void flash_protect_enable();

/**
 * @brief disable flash protection error interrupt
 * @note should be called before jumping to the application
 */
void flash_protect_disable_irq();

#else // ENABLE_FLASH_PROTECTION == 0

#define flash_protect_enable()
#define flash_protect_disable_irq()

#endif // ENABLE_FLASH_PROTECTION

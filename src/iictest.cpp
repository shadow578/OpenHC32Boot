#include "iictest.h"
#include "ui/ui.h"

#include <Wire.h>

// from marlin pins_Aquila_V101.h
#define IIC_EEPROM_SDA PA11
#define IIC_EEPROM_SCL PA12

TwoWire Wire(&I2C1_config, IIC_EEPROM_SDA, IIC_EEPROM_SCL);

void prompt_user(const char *msg = NULL)
{
    if (msg != NULL)
    {
        printf("%s\n", msg);
    }

    UI::await_button_press();
}

/**
 * based on arduino i2c_scanner sample
 *
 * perform a single scan of the I2C bus, printing results to printf
 */
void iic_scanner()
{
    int i = 1;
    int devices = 0;

    for (uint8_t address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();

        if (error == I2C_SUCCESS)
        {
            printf("I2C device found at address 0x%02X\n", address);
            devices++;
        }
        else
        {
            printf("Error %d at address 0x%02X\n", error, address);
        }

        delay(5);
    }

    printf("I2C scan complete, %d devices found\n", devices);
}

/**
 * dumps the first 256 bytes of a 24c16 eeprom at address 0x50
 */
void iic_24c16_dump()
{
#define EEPROM_ADDR 0x50

    for (uint16_t address = 0; address < 0xFF; address += 16)
    {
        // send address
        Wire.beginTransmission(EEPROM_ADDR);
        Wire.write((uint8_t)(address >> 8));
        Wire.write((uint8_t)(address & 0xFF));
        TwoWireStatus stat = Wire.endTransmission();
        printf("stat=%d\n", stat);

        // read 1 bytes
        uint8_t avail = Wire.requestFrom(EEPROM_ADDR, 1);
        printf("avail=%d\n", avail);
        if (Wire.available() > 0)
        {
            uint8_t data = Wire.read();
            printf("%02X: %02X\n", address, data);
        }
        else
        {
            printf("ERR\n");
            break;
        }

        delay(5);
    }
}

/**
 * i2c test suite entry point.
 * called after serial / ui init
 */
void iic_test_entry()
{
    Wire.begin();

    prompt_user("iic_scanner next");
    iic_scanner();

    prompt_user("iic_24c16_dump next");
    iic_24c16_dump();

    prompt_user("iic_tests done");
}
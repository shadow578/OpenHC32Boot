# OpenHC32Boot

Welcome to OpenHC32Boot, an bootloader designed for the HC32F460 series of microcontrollers. This bootloader offers firmware update capabilities via an SD card, support for DWIN screens with a progress display during updates, and features flash protection to ensure only safe parts of flash can be written to.

> [!WARNING]
> This project is currently in an experimental stage. Flashing the bootloader might cause your microcontroller to become unresponsive, and the bootloader itself may not function as expected.

## Features

### Firmware Update via SD Card

OpenHC32Boot allows you to update the firmware of your HC32F460 microcontroller easily using an SD card. This feature streamlines the update process and provides a convenient method for users to keep their devices up-to-date.

### DWIN Screen Support

The bootloader includes support for DWIN screens, enhancing the user experience during firmware updates. A progress display is implemented to keep users informed about the update process, reducing uncertainty and providing feedback on the ongoing operation.

### Flash Protection

To ensure the integrity and safety of your microcontroller's flash memory, OpenHC32Boot incorporates flash protection mechanisms. These measures prevent unauthorized or unsafe writes to critical areas of the flash, minimizing the risk of unintended modifications that could lead to system instability.

## Getting Started

1. Clone the OpenHC32Boot repository to your development environment.

   ```bash
   git clone https://github.com/shadow578/OpenHC32Boot.git
   ```

2. Configure the bootloader according to your specific hardware.

3. Build the bootloader using platformio.

4. Flash the compiled bootloader to your HC32F460 microcontroller.

## Contributing

Contributions to OpenHC32Boot are welcome! If you encounter issues, have suggestions for improvements, or would like to contribute new features, please open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE.md).

## Disclaimer

OpenHC32Boot is an experimental project, and the developers make no guarantees about its stability or suitability for any particular purpose. Use this bootloader at your own risk, and carefully assess its performance and reliability in your specific use case.

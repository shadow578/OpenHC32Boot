# OpenHC32Boot

<p align="center">
    <!--
    <a href="https://github.com/shadow578/OpenHC32Boot/pulse" alt="Activity">
        <img src="https://img.shields.io/github/commit-activity/m/shadow578/OpenHC32Boot" />
    </a>
    -->
    <a href="https://github.com/shadow578/OpenHC32Boot/actions/workflows/ci.yaml">
        <img src="https://github.com/shadow578/OpenHC32Boot/actions/workflows/ci.yaml/badge.svg?branch=main" alt="ci status">
    </a>
</p>

OpenHC32Boot is a bootloader for the HC32F460 series of microcontrollers, allowing firmware to be updated with ease.

## Features

OpenHC32Boot improves on the features found in common stock bootloaders.
Features include:

### Firmware Update via SD Card 💾

Firmware updates are now possible by simply placing a firmware binary on a SD card.
Thus, firmware updates are fairly effortless.

### Screen Support 🖥️

OpenHC32Boot has support for progress output on common 3D-Printer screens, giving you real-time feedback on the boot and update progress.

Currently supporting these screens:
- DWIN screens (as used by Voxelab Aquila X2).

### Re-Flash Protection 🛡️

By verfifying the firmware integrity and hash before starting the update, you will never accidentially re-flash the same firmware binary.
This prevents unnecessary flash cycles.

### Anti-Bricking Features 🧱

OpenHC32Boot prevents overwriting itself during updates and safeguards critical flash areas, ensuring your device will not be bricked even with a failed update.

### User Application Verification ✅

User applications are verified to have reasonable stack, reset and interrupt vectors, ensuring incompatible or broken applications are not booted.

### Reduced Flash Usage 📉

OpenHC32Boot was developed from the ground up for minimal flash usage, leaving more space for the user application.
In practice, OpenHC32Boot consistently uses less flash than common stock bootloaders shipped with most HC32 3D-Printers, even with all features enabled.


## Installation

### Prerequisites 📋

To install OpenHC32Boot, you'll need:

- HC32F460-based 3D printer or development board
- VSCode with PlatformIO installed
- CMSIS-DAP compatible SWD debug probe (e.g., [Raspberry Pi Debug Probe](https://github.com/raspberrypi/debugprobe))

> [!NOTE]
> other debug probes may work as long as they are supported by [pyOCD](https://github.com/pyocd/pyOCD) 

### Installation Steps 📝

1. Backup your current bootloader so you can revert if needed.
2. Clone or download the OpenHC32Boot repository.
3. Open it in VSCode, ensure PlatformIO is installed and active.
4. Configure the bootloader by editing `config.h` and `config_adv.h`.
5. Select the appropriate build environment in PlatformIO.
6. Upload the bootloader using PlatformIO and your debug probe.
7. Reboot your device into the new bootloader, ready for firmware updates!


> [!TIP]
> If your board is not supported, you may need to add a new board configuration. 
> Use existing configurations as a reference, while referring to Marlins board definition file for your board (if it happens to be a 3D-printer).

> [!CAUTION]
> Always backup your current bootloader so you can revert if needed!


## Contributing

Any contributions to OpenHC32Boot are welcome!
Whether you've found a bug, have an improvement idea, or want to add new features, feel free to open an issue or submit a pull request.


## Disclaimer

OpenHC32Boot comes with no guarantees of stability or suitability for specific purposes.
Use it at your own risk, and evaluate its performance in your particular use case. 
Support is provided on a best-effort basis, and developers may not be able to address all issues.


## License

OpenHC32Boot is licensed under [GPL-3.0](LICENSE.md).

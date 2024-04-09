# OpenHC32Boot 🚀

<p align="center">
    <a href="https://github.com/shadow578/OpenHC32Boot/pulse" alt="Activity">
        <img src="https://img.shields.io/github/commit-activity/m/shadow578/OpenHC32Boot" />
    </a>
    <a href="https://github.com/shadow578/OpenHC32Boot/actions/workflows/ci.yaml">
        <img src="https://github.com/shadow578/OpenHC32Boot/actions/workflows/ci.yaml/badge.svg?branch=main" alt="ci status">
    </a>
</p>

Welcome to OpenHC32Boot, your gateway to advanced firmware updates for the HC32F460 series of microcontrollers! 🎉


## Features 🛠️

OpenHC32Boot is more than just a bootloader; it's a powerhouse of features tailored to enhance your HC32F460 microcontroller experience. Here's what it offers:

### Firmware Update via SD Card 💾

Say goodbye to complicated update procedures! OpenHC32Boot simplifies firmware updates by allowing you to simply place a firmware binary on an SD card. Keep your device up-to-date effortlessly.

### Screen Support 🖥️

Stay informed throughout the update process with our screen support feature. Get real-time feedback and updates on your device's status. 

Currently supporting these screens:
- DWIN screens (as used by Voxelab Aquila X2).

### Re-Flash Protection 🛡️

Never accidentally re-flash the same firmware again! OpenHC32Boot verifies firmware integrity to ensure efficient use of flash memory and prevent unnecessary cycles.

### Anti-Bricking Features 🧱

We've designed OpenHC32Boot to be nearly unbrickable. Our bootloader prevents overwriting during updates and safeguards critical flash areas, ensuring your device stays safe even during erroneous updates.

### User Application Verification ✅

Protect your device from malfunctioning or incompatible applications. OpenHC32Boot verifies user applications before booting, ensuring a smooth and responsive user experience every time.

### Reduced Flash Usage 📉

All these features come with minimal flash usage. OpenHC32Boot is designed to be lightweight and efficient, leaving more space for your applications.
In practice, OpenHC32Boot consistently uses less flash than the stock bootloaders shipped with most HC32F460 devices, even with all features enabled.

## Installation 🛠️

### Prerequisites 📋

To install OpenHC32Boot, you'll need:

- HC32F460-based 3D printer or development board
- VSCode with PlatformIO installed
- CMSIS-DAP compatible SWD debug probe (e.g., [Raspberry Pi Debug Probe](https://github.com/raspberrypi/debugprobe))

> [!NOTE]
> other debug probes may work as long as they are supported by [pyOCD](https://github.com/pyocd/pyOCD) 

### Steps 📝

Follow these simple steps to install OpenHC32Boot:

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

## Contributing 🤝

We welcome contributions to OpenHC32Boot! Whether you've found a bug, have an improvement idea, or want to add new features, feel free to open an issue or submit a pull request.

## Disclaimer ⚠️

While we strive for excellence, please understand that OpenHC32Boot comes with no guarantees of stability or suitability for specific purposes. Use it at your own risk, and evaluate its performance in your particular use case. Support is provided on a best-effort basis, and developers may not be able to address all issues.

## License 📄

OpenHC32Boot is licensed under [GPL-3.0](LICENSE.md).


---

README authored by ChatGPT 🤖

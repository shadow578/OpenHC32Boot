import sys
from os import path
import re

Import("env")

def check_fwid(source, target, env):
    """Check the Firmware ID of the binary."""
    firmware_bin = target[0].get_abspath()

    def read_fwid(bin_path: str) -> dict:
        """Read and Parse the Firmware ID from the binary."""

        # firmware id is matched with the following regex:
        # - first group: 32-bit hex number, APP_BASE_ADDRESS
        # - second group: firmware version string
        fwid_regex = r"OpenHC32Boot A:0x([0-9A-F]+) V:(.+)"

        # read binary as a string, search for a string match
        with open(bin_path, "rb") as f:
            content = f.read().decode("ascii", errors="replace")
        
        match = re.search(fwid_regex, content)
        if match is None:
            return None
        
        return {
            "fwid": match.group(0),
            "app_base_address": int(match.group(1), 16),
            "bootloader_version": match.group(2)
        }
    
    # read the firmware id from the binary
    fwid = read_fwid(firmware_bin)
    if fwid is None:
        sys.stderr.write(f"Failed to read FWID from '{firmware_bin}'\n")
        sys.exit(1)
    
    # print firmware id information
    print(f"Read Firmware ID '{fwid['fwid']}' from '{firmware_bin}'")

    # ensure file_size(firmware.bin) <= APP_BASE_ADDRESS
    firmware_bin_size = path.getsize(firmware_bin)

    if firmware_bin_size > fwid["app_base_address"]:
        sys.stderr.write(f"Error: Firmware size ({hex(firmware_bin_size)}) is too large for APP_BASE_ADDRESS={hex(fwid['app_base_address'])}\n")
        
        # calculate minimum required APP_BASE_ADDRESS
        min_app_base_address = 0
        while min_app_base_address < firmware_bin_size:
            min_app_base_address += 0x2000
        
        sys.stderr.write(f"Error: Set APP_BASE_ADDRESS to {hex(min_app_base_address)}\n")

        sys.exit(1)

env.AddPostAction(
    path.join(env.get("BUILD_DIR"), env.get("PROGNAME") + ".bin"),
    env.VerboseAction(check_fwid, "Checking FWID")
)

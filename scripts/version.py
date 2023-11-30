# based on https://community.platformio.org/t/how-to-build-got-revision-into-binary-for-version-output/15380/5
import subprocess
Import("env")

def get_version_build_flags():
    # ensure git is available and this is a git repo
    if subprocess.call(["git", "status"], stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL) != 0:
        raise Exception("cannot get version info: not a git repository or git not installed")

    # get branch name
    # $ git rev-parse --abbrev-ref HEAD
    branch = subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"], text=True).strip()

    # get commit hash
    # $ git rev-parse --short HEAD
    commit_hash = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"], text=True).strip()

    # check if dirty
    # $ git diff-index --quiet HEAD --
    # -> exit code 0 if clean, 1 if dirty
    is_dirty = subprocess.call(["git", "diff-index", "--quiet", "HEAD", "--"], stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL)

    # build version string:
    # - branch name, max 10 chars
    # - first 7 chars of commit hash
    # - '+' if dirty
    version = f"{branch[:10]}-{commit_hash[:7]}{'' if not is_dirty else '+'}"

    print(f"resolved version: {version} ({branch}, {commit_hash}, {'dirty' if is_dirty else 'clean'})")
    return [
        f"-D BOOTLOADER_VERSION='\"{version}\"'",
        f"-D BOOTLOADER_VERSION_BRANCH='\"{branch}\"'",
        f"-D BOOTLOADER_VERSION_COMMIT_HASH='\"{commit_hash}\"'",
        f"-D BOOTLOADER_VERSION_IS_DIRTY={is_dirty}"
    ]

# update build flags
env.Append(
    BUILD_FLAGS=[
        *get_version_build_flags()
    ]
)

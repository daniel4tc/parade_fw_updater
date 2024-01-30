# touch-updater
Tool for updating the firmware on Parade Technologies Touch devices on a Linux host (e.g., Chrome OS).

## Requirements
1. Linux Operating System (on both target and host devices).
2. x86\_64 (Intel) or ARM platform on the target device.
3. If you are cross-compling an ARM build, then an appropriate toolchain must be installed/available on the host machine. We recommend using <a href="https://buildroot.org/">Buildroot</a>.
4. I2C-HID Linux driver on the target device.

## Compilation

### x86\_64 (Intel) Target Platform
If you only need a build for x86\_64, then just run `make all` command in the root directory of this project. Upon successful completion, the binary artifact will be available at `bin/ptupdater`.

### ARM Target Platform (Cross-compilation)
If you are cross-compiling a build for an ARM target platform, and if you are using the <a href="https://buildroot.org/">Buildroot</a> toolchain, then we recommend that you simply run the `build.sh` bash script -- this will build for both x86\_64 and ARM. Binaries will be found at `out/intel/ptupdater` and `out/arm/ptupdater` for x86\_64 and ARM, respectively.
**NOTE:** Ensure that you have your Buildroot root directory in, or symlinked to your `$HOME` directory on the host machine you are building on (i.e., `~/buildroot/` must exist).

## Usage
Please refer to the help output for usage instructions. i.e., via `./ptupdater --help` or just `./ptupdater`.


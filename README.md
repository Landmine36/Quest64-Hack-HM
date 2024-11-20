# Quest64-hack

A repo for building hacks for Quest64 using C/mips assembly

# Building

The building process currently uses a simple batch file</br>
Follow this tutorial by shygoo for setting up armips https://hack64.net/Thread-Importing-C-Code-Into-SM64-Using-n64chain-and-armips</br>
Once armips is set up, clone this repo and create a new folder in the root of the repo named `rom`</br>
Place a vanilla, quest64 rom named `quest64.z64` in the `rom` folder</br>
Running `make` will generate your new modded rom in the `rom` folder named `quest64.mod.z64`

## Installation

### Install dependencies

- Install `python` to PATH.
- Install python dependencies: `pip install -r ./tools/requirements.txt`
- Install `make` to PATH.

- Download the latest release of n64chain for your distro:
  https://github.com/tj90241/n64chain/releases
  (NOTE: is this missing `n64crc`??)
- Download the latest release of `armips` for your distro:
  https://github.com/Kingcom/armips/releases
- Extract `n64chain` to a known directory (referenced herein as `<n64chaindir>`), and then extract armips into the n64chain subdirectory `<n64chaindir>/bin/armips.exe`
- Add `<n64chaindir>/bin` to your PATH.

- Restart all shells that have stale PATH.

### Setup workspace

- Create a new folder in the root of the repo named `rom`.
- Place a vanilla quest64 rom named `quest64.z64` in the `rom` folder.

## Compiling Output

- `make`
- A new file will appear at `rom/quest64.mod.z64`.

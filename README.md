# Aqua-mixtura
Wasseraufbereitung fürs Bierbrauen

Water treatment for brewing beer

## Build from source

### Using Qt Creator

* Download and install "Qt Framework and Tools" from https://www.qt.io/download-dev
* Open this repository in qt creator and build it

### Using CMake

The following software must be installed to build Aqua-mixtura:
* c/c++ compiler
* cmake
* Qt6 library with development header

For Debian/Ubuntu install these packages:
```sh
sudo apt install git build-essential cmake qt6-base-dev qt6-tools-dev
```

For Fedora install these packages:
```sh
sudo dnf install lsb_release git cmake make automake gcc gcc-c++ qt6-qtbase-devel qt6-linguist qt6-qttools-devel rpm-build
```

For openSUSE install these packages:
```sh
sudo zypper install lsb-release git cmake make automake gcc gcc-c++ rpm-build qt6-base-devel qt6-tools-linguist qt6-linguist-devel
```

Then use cmake to build it:
```sh
git clone https://github.com/jo-hannes/Aqua-mixtura.git
cd Aqua-mixtura
mkdir build
cd build
cmake ..
make
```



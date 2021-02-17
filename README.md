# 3D_Printer
Project of Fully functional 3D Printer in C language on STM32 Nucleo-F303RE board.

# Required Software
- WSL
- cmake
- arm-none-eabi-gcc
- STM32LinkUtility
- STM32Qube
- build-essential (UnitTests)
- git (UnitTests)

# Software installation
```
(...)
sudo apt-get install cmake
sudo apt-get install gcc-arm-none-eabi
(...)
```
Additionally for UnitTests:
```
sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install git
(...)
```

# Application Build
```
mkdir bld
cd bld
cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake ..
make
```

# UnitTests build and run
All commands should be run inside UnitTest folder:
```
cd UnitTests
```
Download googletest framework:
```
git clone https://github.com/google/googletest.git
```
Build and run Unit Tests:
```
mkdir bld
cd bld
cmake ..
make
ctest
```

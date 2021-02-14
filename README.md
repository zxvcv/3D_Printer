# 3D_Printer
Project of Fully functional 3D Printer in C language on STM32 Nucleo-F303RE board.

# Required Software
WSL
arm-none-eabi-gcc
STM32LinkUtility
STM32Qube

# Software installation
(...)
sudo apt-get install gcc-arm-none-eabi
(...)

# Compilation
mkdir bld
cd bld
cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake ..
make
# 3D_Printer
Project of fully functional 3D Printer in C language on STM32 Nucleo-F303RE board.

# Basic Informations
- Code is written without attachments to any IDE (VSCode will be supported)

# Software installation
Right now the only Fully supported platform is Windows & WSL.

&nbsp;

Installation steps:

1. WSL installation:

The entire WSL installation process is described in the links below (WSL2 recommended):
- https://docs.microsoft.com/en-us/windows/wsl/install-win10#manual-installation-steps
- https://docs.microsoft.com/en-us/windows/wsl/install-manual

The second option is use of powershell script from below repository (read license before use):
- https://github.com/zxvcv/WinLibManagement/blob/main/src/Setup-WSL.ps1

&nbsp;

2. Linux packages installation on WSL:

Packages necessary for build:
```
sudo apt-get update

sudo apt-get install git
sudo apt-get install cmake
sudo apt-get install gcc-arm-none-eabi
```
Additionally packages necessary for UnitTests:
```
sudo apt-get update

sudo apt-get install build-essential
```

&nbsp;

3. Software for board flashing (optionally):

To flash the board on Windowns & WSL we need to install STM32 ST-LINK Utility:
- https://www.st.com/en/development-tools/stsw-link004.html

&nbsp;

4. VSCode (optionally):

Recommended by me way to install VSCode and any other software on windows system is by using choco package manager.
- https://chocolatey.org/install

After choco installation you can install VSCode in Unix-like way from PowerShell:
```
choco install vscode
```

Extensions for VSCode to install:
```
Remote - WSL
CMake           (inside WSL)
CMake Tools     (inside WSL)
Cortex-Debug    (inside WSL, optionally for debugging)
```

If you want to use flashing board from VSCode, step 3 is necessary to continue.

Update path to ST-LINK_CLI.exe in VSCode falsh task inside .vscode/tasks.json (update "command" field with your absolute path to ST-LINK_CLI.exe file).



# Application build

Command line (cwd: \<main project folder\>):
```
mkdir bld
cd bld
cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake ..
make
```
VSCode:
```
<Ctrl+Shift+B>
select: <build>
select: <Continue without scanning the task output>
```

# UnitTests build and run
Before first run of the UnitTests, download googletest framework (cwd: \<main project folder\>/UnitTests):
```
git clone https://github.com/google/googletest.git
```

Command line (cwd: \<main project folder\>/UnitTests):
```
mkdir bld
cd bld
cmake ..
make
ctest
```

VSCode:
```
<Ctrl+Shift+B>
select: <UT_build>
select: <Continue without scanning the task output>
```

# Flashing
Command line:

For flashing there is prepared script in PowerShell in tools/flashing/Flash-Board.ps1

VSCode:
```
<Ctrl+Shift+B>
select: <flash>
select: <Continue without scanning the task output>
```

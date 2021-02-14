###########################################################################################
#   Function to setup workspace for 3D_Printer project
#
###########################################################################################
#   Example:
#       Set-ExecutionPolicy Bypass -Scope Process -Force; .\SetupWorkspace.ps1
###########################################################################################

$thisScriptPath = $script:MyInvocation.MyCommand.Path
$root = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

# Place where libraries are stored (https://github.com/zxvcv/WinLibManagement)
$libs = "\WinLibManagement\src"

if($libs -eq "")
{
    $libs = $root
}

. $libs\Setup-WSL.ps1
. $libs\Setup-STM32Qube.ps1
. $libs\Setup-STM32LinkUtility.ps1

# Chocolatey
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))

# WSL2
if($args.Length -eq 0)
{
    Setup-WSL -startPoint "begin" -restoreScript $thisScriptPath
}

if($args.Length -eq 1 -and ($args[0] -eq "afterWinUpdate_WSL" -or $args[0] -eq "afterVMEN_WSL"))
{
    Setup-WSL -startPoint $args[0]
}

choco install googlechrome -y
choco install 7zip.install -y
choco install vscode -y
choco install notepadplusplus.install -y
choco install git.install -y
choco install termite -y
choco install python --version=3.8.7 -y
choco install microsoft-windows-terminal -y
#choco install wsl-ubuntu-2004 -y # Ubuntu 20.04 for WSL (its already installed by Setup-WSL)
choco install javaruntime -y
Setup-STM32Qube
Setup-STM32LinkUtility

Read-Host "Press ENTER to continue..."
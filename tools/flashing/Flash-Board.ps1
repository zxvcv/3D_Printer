###########################################################################################
#   Function flash build program on STM32 board using ST-LINK Utility
#
###########################################################################################
#   Example:
#       Flash-Board
###########################################################################################

function Flash-Board()
{
    param(
        [parameter(Mandatory=$false, Position=0)][string] $hex_file = ""
    )

    BEGIN
    {
        $thisScriptPath = $script:MyInvocation.MyCommand.Path

        if($hex_file -eq "")
        {
            $hex_file = Join-Path -Path (Split-Path $thisScriptPath -Parent)`
                                  -ChildPath "../../bld/3D_Printer.hex"
        }
    }

    PROCESS
    {
        if(-Not (Test-Path -Path $hex_file))
        {
            Write-Warning "HEX file does not exit. Check if you build code before."
            Write-Error "ABORTING..." -Category InvalidData
            exit
        }

        st-link_cli -P $hex_file -V
    }

    END{}
}

Flash-Board $args[0]

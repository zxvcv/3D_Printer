cmake_minimum_required(VERSION 3.10.2)

set(This 3D_Printer_Embedded)

project(${This} VERSION 0.1)

enable_testing()

add_subdirectory(googletest)


#test will be a program
#tested code will be a library for this program


#***** tests *****#
add_subdirectory(Tests/fifo)
add_subdirectory(Tests/vectorOperations)
add_subdirectory(Tests/a4988_stepstick)
add_subdirectory(Tests/EEPROM_24AA01)
add_subdirectory(Tests/parserCommand)
#add_subdirectory(Tests/managerBT)

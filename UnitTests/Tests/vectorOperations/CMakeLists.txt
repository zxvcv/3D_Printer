cmake_minimum_required(VERSION 3.10.2)

set(This vectorOperations_UT)


set(Sources
    vectorOperations_UT.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    3D_Printer_Embedded
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

function(set_target_compile_options TARGET)
    target_compile_options(${TARGET} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections

        -Wall

        $<$<CONFIG:Debug>:-Og>
    )
endfunction()


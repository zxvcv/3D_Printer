# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

function(set_target_compile_definitions TARGET)
    target_compile_definitions(${TARGET} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F303xE
    )
endfunction()


add_library(stm32 OBJECT)

set(INCLUDE_DIRS_STM
    cmsis_device_stm_l4/Include
    cmsis_5/CMSIS/Core/Include
    stm32l4xx_hal_driver/Inc
)

set(SOURCE_STM
    stm32l4xx_hal_driver/Src/stm32l4xx_hal.c
    stm32l4xx_hal_driver/Src/stm32l4xx_hal_cortex.c
    stm32l4xx_hal_driver/Src/stm32l4xx_hal_gpio.c
)

target_include_directories(stm32 PUBLIC ${INCLUDE_DIRS_STM})
target_sources(stm32 PRIVATE ${SOURCE_STM})

set(INCLUDE_DIRS_QPCPP
    qpcpp/include
    qpcpp/ports/arm-cm/qv/gnu
)
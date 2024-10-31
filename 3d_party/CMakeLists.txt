add_library(stm32 OBJECT)
add_library(qpcpp OBJECT)

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
    qpcpp/ports/arm-cm/qv/config
)

set(QP_CPP_SRCS
    qpcpp/src/qf/qep_hsm.cpp
    qpcpp/src/qf/qep_msm.cpp
    qpcpp/src/qf/qf_act.cpp
    qpcpp/src/qf/qf_actq.cpp
    qpcpp/src/qf/qf_defer.cpp
    qpcpp/src/qf/qf_dyn.cpp
    qpcpp/src/qf/qf_mem.cpp
    qpcpp/src/qf/qf_ps.cpp
    qpcpp/src/qf/qf_qact.cpp
    qpcpp/src/qf/qf_qeq.cpp
    qpcpp/src/qf/qf_qmact.cpp
    qpcpp/src/qf/qf_time.cpp
    qpcpp/src/qv/qv.cpp
    qpcpp/ports/arm-cm/qv/gnu/qv_port.cpp)

target_include_directories(qpcpp PUBLIC ${INCLUDE_DIRS_QPCPP})
target_sources(qpcpp PRIVATE ${QP_CPP_SRCS})
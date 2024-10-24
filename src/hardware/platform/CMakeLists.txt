set(INCLUDE_DIRS_STM
    .
)

set(SOURCE_STM
    startup_stm32l476xx.s
    stm32l4xx_it.c
    system_stm32l4xx.c
)

target_include_directories(stm32 PUBLIC ${INCLUDE_DIRS_STM})
target_sources(stm32 PRIVATE ${SOURCE_STM})
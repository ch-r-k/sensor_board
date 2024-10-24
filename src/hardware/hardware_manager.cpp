#include "hardware_manager.hpp"

#include "stm32l4xx.h"  // CMSIS-compliant header file for the MCU used
#include "stm32l4xx_hal.h"

HardwareManager::HardwareManager()
{
    // Configure the MPU to prevent NULL-pointer dereferencing ...
    MPU->RBAR = 0x0U                           // base address (NULL)
                | MPU_RBAR_VALID_Msk           // valid region
                | (MPU_RBAR_REGION_Msk & 7U);  // region #7
    MPU->RASR = (7U << MPU_RASR_SIZE_Pos)      // 2^(7+1) region
                | (0x0U << MPU_RASR_AP_Pos)    // no-access region
                | MPU_RASR_ENABLE_Msk;         // region enable
    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk        // enable background region
                | MPU_CTRL_ENABLE_Msk;         // enable the MPU
    __ISB();
    __DSB();

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    // Hal init
    HAL_Init();

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    ledPin.configure(OutputPin::Port::PORT_A, OutputPin::Pin::PIN_5,
                     OutputPin::Mode::PUSH_PULL, OutputPin::Pull::NOPULL,
                     OutputPin::Speed::LOW);
}

HardwareManager::~HardwareManager() {}

OutputPin& HardwareManager::getLedPin() { return ledPin; }
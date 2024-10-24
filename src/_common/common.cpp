#include "common.hpp"
#include <stm32l4xx_hal.h>

void busyWaiting(std::uint32_t delay_ms)
{
    HAL_Delay(delay_ms);
}
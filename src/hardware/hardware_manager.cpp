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

    HAL_Init();

    SystemClock_Config();

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    ledPin.configure(OutputPin::Port::PORT_A, OutputPin::Pin::PIN_5,
                     OutputPin::Mode::PUSH_PULL, OutputPin::Pull::NOPULL,
                     OutputPin::Speed::LOW);

    // Spi
    __HAL_RCC_SPI1_CLK_ENABLE();

    sensorSpi.Configure(Spi::Mode::Master);
    sensorSpi.Configure(Spi::Direction::Direction2Lines);
    sensorSpi.Configure(Spi::DataSize::DataSize8Bit);
    sensorSpi.Configure(Spi::ClockPolarity::PolarityLow);
    sensorSpi.Configure(Spi::ClockPhase::Phase1Edge);
    sensorSpi.Configure(Spi::Nss::Soft);
    sensorSpi.Configure(Spi::BaudratePrescaler::preScale256);
    sensorSpi.Configure(Spi::FirstBit::MSB);
    // sensorSpi.Configure(Spi::T);
    sensorSpi.Configure(Spi::CRCCalculation::Disable);
    sensorSpi.Configure(Spi::NssPMode::PulseEnable);

    sensorI2c.ConfigureTiming(0x10909CEC);  // todo
    sensorI2c.Configure(I2c::AddressingMode::SevenBit);
    sensorI2c.Configure(I2c::DualAddressMode::Disable);
    sensorI2c.Configure(I2c::GeneralCallMode::Disable);
    sensorI2c.Configure(I2c::NoStretchMode::Disable);
}

void HardwareManager::SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};

    /** Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
    }

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
    }
}

HardwareManager::~HardwareManager() {}

OutputPin& HardwareManager::getLedPin() { return ledPin; }

Spi& HardwareManager::getSensorSpi() { return sensorSpi; }

I2c& HardwareManager::getSensorI2c() { return sensorI2c; }
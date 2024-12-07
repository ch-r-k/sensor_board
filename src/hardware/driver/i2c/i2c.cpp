#include "i2c.hpp"
#include <cassert>
#include <cstdint>
#include "driver/interrupt_dispatcher/interrupt.hpp"
#include "qpcpp.hpp"

std::unordered_map<I2C_TypeDef *, I2c *> I2c::instanceMap;

I2c::I2c(std::uint8_t instance)
{
    assert(instance >= 1 && instance <= 3);

    static constexpr I2C_TypeDef *i2cInstances[] = {I2C1, I2C2, I2C3};

    handler.Instance = i2cInstances[instance - 1];
    RegisterInstance();

    switch (instance)
    {
        case 1:
            gpioClk.Pin = GPIO_PIN_6;
            gpioSData.Pin = GPIO_PIN_7;

            portClk = GPIOB;
            portSData = GPIOB;

            gpioClk.Alternate = GPIO_AF4_I2C1,
            gpioSData.Alternate = GPIO_AF4_I2C1;

            break;

        case 2:
            gpioClk.Pin = GPIO_PIN_11;
            gpioSData.Pin = GPIO_PIN_13;

            portClk = GPIOB;
            portSData = GPIOB;

            gpioClk.Alternate = GPIO_AF4_I2C2,
            gpioSData.Alternate = GPIO_AF4_I2C2;

            break;

        case 3:
            gpioClk.Pin = GPIO_PIN_0;
            gpioSData.Pin = GPIO_PIN_1;

            portClk = GPIOC;
            portSData = GPIOC;

            gpioClk.Alternate = GPIO_AF4_I2C3,
            gpioSData.Alternate = GPIO_AF4_I2C3;

            break;

        default:
            break;
    }
}

I2c::~I2c()
{
    handler.Instance = nullptr;
    UnregisterInstance();
}

void I2c::Open()
{
    assert((open == false) && "already open");
    assert(handler.Instance != nullptr && "instance can't be nullptr");

    if (handler.Instance == I2C1)
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
    else if (handler.Instance == I2C2)
    {
        __HAL_RCC_I2C2_CLK_ENABLE();
    }
    else if (handler.Instance == I2C3)
    {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }

    gpioClk.Mode = GPIO_MODE_AF_PP;
    gpioClk.Pull = GPIO_NOPULL;
    gpioClk.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    gpioSData.Mode = GPIO_MODE_AF_PP;
    gpioSData.Pull = GPIO_NOPULL;
    gpioSData.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(portClk, &gpioClk);
    HAL_GPIO_Init(portSData, &gpioSData);

    Interrupt &dispatcher = Interrupt::getInstance();

    if (handler.Instance == I2C1)
    {
        dispatcher.registerInterrupt(this, &I2c::Isr, I2C1_EV_IRQn);

        HAL_NVIC_SetPriority(I2C1_EV_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    }
    else if (handler.Instance == I2C2)
    {
        dispatcher.registerInterrupt(this, &I2c::Isr, I2C2_EV_IRQn);

        HAL_NVIC_SetPriority(I2C2_EV_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    }
    else if (handler.Instance == I2C3)
    {
        dispatcher.registerInterrupt(this, &I2c::Isr, I2C3_EV_IRQn);

        HAL_NVIC_SetPriority(I2C3_EV_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    }

    if (HAL_I2C_Init(&handler) != HAL_OK)
    {
        assert(false && "Failed to initialize SPI");
    }

    open = true;
}

void I2c::Close()
{
    assert(open == false && "must be open");

    if (HAL_I2C_DeInit(&handler) != HAL_OK)
    {
        assert(false && "Failed to initialize I2C");
    }
}

void I2c::SetIcb(IcbI2c &icb_i2c) { icbI2c = &icb_i2c; }

void I2c::StartWrite(const std::span<const std::uint8_t> data)
{
    assert(open && "I2C must be opened before writing");

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_IT(
        &handler, address, const_cast<uint8_t *>(data.data()), data.size());

    assert(status == HAL_OK && "I2C Write failed");
}

void I2c::StartRead(const std::span<std::uint8_t> data)
{
    assert(open && "I2C must be opened before reading");

    HAL_StatusTypeDef status =
        HAL_I2C_Master_Receive_IT(&handler, address, data.data(), data.size());

    assert(status == HAL_OK && "I2C Read failed");
}

void I2c::SetAddress(const std::uint8_t address) { this->address = address; }

void I2c::ConfigureTiming(std::uint32_t timing)
{
    handler.Init.Timing = timing;
}

void I2c::Configure(AddressingMode addressingMode)
{
    handler.Init.AddressingMode = static_cast<std::uint32_t>(addressingMode);
}

void I2c::Configure(DualAddressMode dualAddressMode)
{
    handler.Init.DualAddressMode = static_cast<std::uint32_t>(dualAddressMode);
}

void I2c::Configure(GeneralCallMode generalCallMode)
{
    handler.Init.GeneralCallMode = static_cast<std::uint32_t>(generalCallMode);
}

void I2c::Configure(NoStretchMode noStretchMode)
{
    handler.Init.NoStretchMode = static_cast<std::uint32_t>(noStretchMode);
}

extern "C" void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    I2c::RxISR(hi2c);
}

extern "C" void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    I2c::TxISR(hi2c);
}

extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    I2c::TxISR(hi2c);
}
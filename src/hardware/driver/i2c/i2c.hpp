#ifndef I2C_HPP
#define I2C_HPP

#include "i_i2c.hpp"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include <cassert>
#include <span>

class I2c : public II2c
{
   public:
    enum class AddressingMode : std::uint32_t
    {
        SevenBit = I2C_ADDRESSINGMODE_7BIT,
        TenBit = I2C_ADDRESSINGMODE_10BIT
    };

    enum class DualAddressMode : std::uint32_t
    {
        Disable = I2C_DUALADDRESS_DISABLE,
        Enable = I2C_DUALADDRESS_ENABLE
    };

    enum class GeneralCallMode : std::uint32_t
    {
        Disable = I2C_GENERALCALL_DISABLE,
        Enable = I2C_GENERALCALL_ENABLE
    };

    enum class NoStretchMode : std::uint32_t
    {
        Disable = I2C_NOSTRETCH_DISABLE,
        Enable = I2C_NOSTRETCH_ENABLE
    };

   private:
    I2C_HandleTypeDef handler;
    bool open = false;
    std::uint8_t address;

   public:
    I2c(std::uint8_t instance);
    ~I2c();

    GPIO_InitTypeDef gpioClk = {};
    GPIO_TypeDef* portClk = nullptr;

    GPIO_InitTypeDef gpioSData = {};
    GPIO_TypeDef* portSData = nullptr;

    void Open() override;
    void Close() override;

    void StartWrite(const std::span<const std::uint8_t> data) override;

    void StartRead(const std::span<std::uint8_t> data) override;

    void ConfigureTiming(std::uint32_t timing);
    void Configure(AddressingMode addressingMode);
    void Configure(DualAddressMode dualAddressMode);
    void Configure(GeneralCallMode generalCallMode);
    void Configure(NoStretchMode noStretchMode);

    void setAddress(std::uint8_t address);

    static void Isr(void* callbackObject, [[maybe_unused]] void* parameter)
    {
        // Cast callbackObject to Spi* and call the non-static ISR
        I2c* i2c = static_cast<I2c*>(callbackObject);

        HAL_I2C_EV_IRQHandler(&i2c->handler);
    }

    // ISR functions for handling I2C interrupts
    static void RxISR(I2C_HandleTypeDef* hi2c)
    {
        // Implement the RX interrupt handling logic here
        if (hi2c->Instance == I2C1)
        {
            // Handle I2C1 RX interrupt
        }
        else
        {
            assert(false && "not implemented");
        }
    }

    static void TxISR(I2C_HandleTypeDef* hi2c)
    {
        // Implement the TX interrupt handling logic here
        if (hi2c->Instance == I2C1)
        {
            // Handle I2C1 TX interrupt
        }
        else
        {
            assert(false && "not implemented");
        }
    }
};

#endif  // I2C_HPP

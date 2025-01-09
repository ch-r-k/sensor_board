#ifndef I2C_HPP
#define I2C_HPP

#include "i_i2c.hpp"
#include "icb_i2c.hpp"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include <cassert>
#include <span>
#include <unordered_map>

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
    IcbI2c* icbI2c = nullptr;
    static std::unordered_map<I2C_TypeDef*, I2c*> instanceMap;

   public:
    I2c(std::uint8_t instance);
    ~I2c();

    GPIO_InitTypeDef gpioClk = {};
    GPIO_TypeDef* portClk = nullptr;

    GPIO_InitTypeDef gpioSData = {};
    GPIO_TypeDef* portSData = nullptr;

    void Open() override;
    void Close() override;
    void SetIcb(IcbI2c& icb_i2c);

    void StartWrite(const std::span<const std::uint8_t> data) override;
    void StartRead(const std::span<std::uint8_t> data) override;
    void SetAddress(const std::uint8_t address) override;

    void ConfigureTiming(std::uint32_t timing);
    void Configure(AddressingMode addressingMode);
    void Configure(DualAddressMode dualAddressMode);
    void Configure(GeneralCallMode generalCallMode);
    void Configure(NoStretchMode noStretchMode);

    void RegisterInstance() { instanceMap[handler.Instance] = this; }

    // Remove the association (optional)
    void UnregisterInstance() { instanceMap.erase(handler.Instance); }

    static void Isr(void* callbackObject, [[maybe_unused]] void* parameter)
    {
        // Cast callbackObject to Spi* and call the non-static ISR
        I2c* i2c = static_cast<I2c*>(callbackObject);

        HAL_I2C_EV_IRQHandler(&i2c->handler);
    }

    // ISR functions for handling I2C interrupts
    static void RxISR(I2C_HandleTypeDef* hi2c)
    {
        auto it = instanceMap.find(hi2c->Instance);
        if (it != instanceMap.end())
        {
            I2c* i2c = it->second;  // Retrieve the I2c instance
            if (i2c->icbI2c)
            {
                i2c->icbI2c->ReadDone();
            }
            else
            {
                assert(false && "icbI2c is null");
            }
        }
        else
        {
            assert(false && "No associated I2c instance found");
        }
    }

    static void TxISR(I2C_HandleTypeDef* hi2c)
    {
        auto it = instanceMap.find(hi2c->Instance);
        if (it != instanceMap.end())
        {
            I2c* i2c = it->second;  // Retrieve the I2c instance
            if (i2c->icbI2c)
            {
                i2c->icbI2c->WriteDone();
            }
            else
            {
                assert(false && "icbI2c is null");
            }
        }
        else
        {
            assert(false && "No associated I2c instance found");
        }
    }
};

#endif  // I2C_HPP

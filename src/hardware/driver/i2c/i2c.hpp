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

namespace hardware_layer
{
class I2c : public II2c
{
   public:
    enum class AddressingMode : std::uint32_t
    {
        SEVEN_BIT = I2C_ADDRESSINGMODE_7BIT,
        TEN_BIT = I2C_ADDRESSINGMODE_10BIT
    };

    enum class DualAddressMode : std::uint32_t
    {
        DISABLE = I2C_DUALADDRESS_DISABLE,
        ENABLE = I2C_DUALADDRESS_ENABLE
    };

    enum class GeneralCallMode : std::uint32_t
    {
        DISABLE = I2C_GENERALCALL_DISABLE,
        ENABLE = I2C_GENERALCALL_ENABLE
    };

    enum class NoStretchMode : std::uint32_t
    {
        DISABLE = I2C_NOSTRETCH_DISABLE,
        ENABLE = I2C_NOSTRETCH_ENABLE
    };

   private:
    I2C_HandleTypeDef handler;
    bool isOpen = false;
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

    void open() override;
    void close() override;
    void setIcb(IcbI2c& icb_i2c);

    void startWrite(const std::span<const std::uint8_t> data) override;
    void startRead(const std::span<std::uint8_t> data) override;
    void setAddress(const std::uint8_t address) override;

    void configureTiming(std::uint32_t timing);
    void configure(AddressingMode addressing_mode);
    void configure(DualAddressMode dual_address_mode);
    void configure(GeneralCallMode general_call_mode);
    void configure(NoStretchMode no_stretch_mode);

    void registerInstance() { instanceMap[handler.Instance] = this; }

    // Remove the association (optional)
    void unregisterInstance() { instanceMap.erase(handler.Instance); }

    static void isr(void* callback_object, [[maybe_unused]] void* parameter)
    {
        // Cast callbackObject to Spi* and call the non-static ISR
        I2c* i2c = static_cast<I2c*>(callback_object);

        HAL_I2C_EV_IRQHandler(&i2c->handler);
    }

    // ISR functions for handling I2C interrupts
    static void rxIsr(I2C_HandleTypeDef* hi2c)
    {
        auto it = instanceMap.find(hi2c->Instance);
        if (it != instanceMap.end())
        {
            I2c* i2c = it->second;  // Retrieve the I2c instance
            if (i2c->icbI2c)
            {
                i2c->icbI2c->readDone();
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

    static void txIsr(I2C_HandleTypeDef* hi2c)
    {
        auto it = instanceMap.find(hi2c->Instance);
        if (it != instanceMap.end())
        {
            I2c* i2c = it->second;  // Retrieve the I2c instance
            if (i2c->icbI2c)
            {
                i2c->icbI2c->writeDone();
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

}  // namespace hardware_layer

#endif  // I2C_HPP

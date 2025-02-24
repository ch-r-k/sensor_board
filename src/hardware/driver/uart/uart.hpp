#ifndef UART_HPP
#define UART_HPP

#include "i_uart.hpp"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include <cassert>
#include <span>

namespace hardware_layer
{
class Uart : public IUart
{
   public:
    enum class Baudrate : std::uint32_t
    {
        SevenBit = I2C_ADDRESSINGMODE_7BIT,
        TenBit = I2C_ADDRESSINGMODE_10BIT
    };

    enum class HardwareControl : std::uint32_t
    {
        None = UART_HWCONTROL_NONE,
        Rts = UART_HWCONTROL_RTS,
        Cts = UART_HWCONTROL_CTS,
        RtsCts = UART_HWCONTROL_RTS_CTS
    };

    enum class OneBitSample : std::uint32_t
    {
        Disable = UART_ONE_BIT_SAMPLE_DISABLED,
        Enable = UART_ONE_BIT_SAMPLE_ENABLE
    };

    enum class Mode : std::uint32_t
    {
        Rx = UART_MODE_RX,
        Tx = UART_MODE_TX,
        TxRx = UART_MODE_TX_RX
    };

    enum class Oversampling : std::uint32_t
    {
        Oversampling8 = UART_OVERSAMPLING_8,
        Oversampling16 = UART_OVERSAMPLING_16
    };

    enum class Parity : std::uint32_t
    {
        None = UART_PARITY_NONE,
        Even = UART_PARITY_EVEN,
        Odd = UART_PARITY_ODD
    };

    enum class StopBits : std::uint32_t
    {
        Bit_0_5 = UART_STOPBITS_0_5,
        Bit_1_0 = UART_STOPBITS_1,
        Bit_1_5 = UART_STOPBITS_1_5,
        Bit_2_0 = UART_STOPBITS_2
    };

    enum class WordLength : std::uint32_t
    {
        Length_7 = UART_WORDLENGTH_7B,
        Length_8 = UART_WORDLENGTH_8B,
        Length_9 = UART_WORDLENGTH_9B
    };

   private:
    UART_HandleTypeDef handler;

    GPIO_InitTypeDef gpioTx = {};
    GPIO_TypeDef* portTx = nullptr;

    GPIO_InitTypeDef gpioRx = {};
    GPIO_TypeDef* portRx = nullptr;

    RCC_PeriphCLKInitTypeDef periphClkInit = {};

    bool open = false;

   public:
    Uart(std::uint8_t instance);
    ~Uart();

    UART_HandleTypeDef* getHandler() { return &handler; }

    void Open() override;
    void Close() override;

    void StartWrite(const std::span<std::uint8_t> data) override;

    void StartRead(const std::span<std::uint8_t> data) override;

    bool isTxBusy();

    void ConfigureBaudrate(std::uint32_t baudrate);
    void Configure(Mode mode);
    void Configure(OneBitSample enable);
    void Configure(Oversampling oversampling);
    void Configure(HardwareControl hardware_control);
    void Configure(Parity parity);
    void Configure(StopBits stop_bits);
    void Configure(WordLength word_length);

    // ISR functions for handling UART interrupts
    // void Isr(UART_HandleTypeDef* huart) { HAL_UART_IRQHandler(huart); }

    static void Isr(void* callbackObject, [[maybe_unused]] void* parameter)
    {
        // Cast callbackObject to Uart* and call the non-static ISR
        Uart* uart = static_cast<Uart*>(callbackObject);

        HAL_UART_IRQHandler(&uart->handler);
    }

    static void RxISR([[maybe_unused]] UART_HandleTypeDef* hUart) {}

    static void TxISR([[maybe_unused]] UART_HandleTypeDef* hUart)
    {
        switch (reinterpret_cast<uintptr_t>(hUart->Instance))
        {
        }
    }
};
}  // namespace hardware_layer

#endif  // UART_HPP

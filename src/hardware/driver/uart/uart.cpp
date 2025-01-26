#include "uart.hpp"
#include <cassert>
#include "driver/interrupt_dispatcher/interrupt.hpp"

namespace hardware_layer
{
Uart::Uart(std::uint8_t instance)
{
    assert(instance >= 1 && instance <= 5);

    static constexpr USART_TypeDef *uartInstances[] = {USART1, USART2, USART3,
                                                       UART4, UART5};

    handler.Instance = uartInstances[instance - 1];

    switch (instance)
    {
        case 1:
            gpioTx.Pin = GPIO_PIN_9;
            gpioRx.Pin = GPIO_PIN_10;

            portRx = GPIOA;
            portTx = GPIOA;

            gpioTx.Alternate = GPIO_AF7_USART1;
            gpioRx.Alternate = GPIO_AF7_USART1;

            periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
            periphClkInit.Usart2ClockSelection = RCC_USART1CLKSOURCE_PCLK2;

            break;

        case 2:
            gpioTx.Pin = GPIO_PIN_2;
            gpioRx.Pin = GPIO_PIN_3;

            portRx = GPIOA;
            portTx = GPIOA;

            gpioTx.Alternate = GPIO_AF7_USART2;
            gpioRx.Alternate = GPIO_AF7_USART2;

            periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
            periphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
            break;

        case 3:
            gpioTx.Pin = GPIO_PIN_10;
            gpioRx.Pin = GPIO_PIN_11;

            portRx = GPIOB;
            portTx = GPIOB;

            gpioTx.Alternate = GPIO_AF8_UART4;
            gpioRx.Alternate = GPIO_AF8_UART4;

            gpioTx.Alternate = GPIO_AF7_USART3;
            gpioRx.Alternate = GPIO_AF7_USART3;

            periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
            periphClkInit.Usart2ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
            break;

        case 4:
            gpioTx.Pin = GPIO_PIN_0;
            gpioRx.Pin = GPIO_PIN_1;

            portRx = GPIOA;
            portTx = GPIOA;

            periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART4;
            periphClkInit.Usart2ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
            break;

        default:
            break;
    }
}

Uart::~Uart() { handler.Instance = nullptr; }

void Uart::Open()
{
    assert(handler.Instance != nullptr && "nullptr");
    assert(open != true && "must be closed");

    if (HAL_RCCEx_PeriphCLKConfig(&periphClkInit) != HAL_OK)
    {
        assert(false && "Failed to initialize UART clock");
    }

    if (handler.Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
    }
    else if (handler.Instance == USART2)
    {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (handler.Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_ENABLE();
    }
    else if (handler.Instance == UART4)
    {
        __HAL_RCC_UART4_CLK_ENABLE();
    }
    else if (handler.Instance == UART5)
    {
        // todo
    }

    gpioTx.Mode = GPIO_MODE_AF_PP;
    gpioTx.Pull = GPIO_NOPULL;
    gpioTx.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    gpioRx.Mode = GPIO_MODE_AF_PP;
    gpioRx.Pull = GPIO_NOPULL;
    gpioRx.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(portTx, &gpioTx);
    HAL_GPIO_Init(portRx, &gpioRx);

    if (HAL_UART_Init(&handler) != HAL_OK)
    {
        assert(false && "Failed to initialize UART");
    }

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    Interrupt &dispatcher = Interrupt::getInstance();

    // Register the USART3 callback
    dispatcher.registerInterrupt(this, &Uart::Isr, USART2_IRQn);

    open = true;
}

void Uart::Close()
{
    assert(open == false && "must be open");

    if (HAL_UART_DeInit(&handler) != HAL_OK)
    {
        assert(false && "Failed to initialize UART");
    }
}

void Uart::StartWrite(const std::span<std::uint8_t> data)
{
    assert(open && "UART must be opened before writing");

    HAL_StatusTypeDef status =
        HAL_UART_Transmit_IT(&handler, data.data(), data.size());

    assert(status == HAL_OK && "UART Write failed");
}

void Uart::StartRead(const std::span<std::uint8_t> data)
{
    assert(open && "UART must be opened before reading");

    HAL_StatusTypeDef status =
        HAL_UART_Receive_IT(&handler, data.data(), data.size());

    assert(status == HAL_OK && "UART Read failed");
}

void Uart::ConfigureBaudrate(std::uint32_t baudrate)
{
    handler.Init.BaudRate = baudrate;
}

void Uart::Configure(HardwareControl hardware_control)
{
    handler.Init.HwFlowCtl = static_cast<std::uint32_t>(hardware_control);
}

void Uart::Configure(Mode mode)
{
    handler.Init.Mode = static_cast<std::uint32_t>(mode);
}

void Uart::Configure(OneBitSample enable)
{
    handler.Init.OneBitSampling = static_cast<std::uint32_t>(enable);
}

void Uart::Configure(Oversampling oversampling)
{
    handler.Init.OverSampling = static_cast<std::uint32_t>(oversampling);
}

void Uart::Configure(Parity parity)
{
    handler.Init.Parity = static_cast<std::uint32_t>(parity);
}

void Uart::Configure(StopBits stop_bits)
{
    handler.Init.StopBits = static_cast<std::uint32_t>(stop_bits);
}

void Uart::Configure(WordLength word_length)
{
    handler.Init.WordLength = static_cast<std::uint32_t>(word_length);
}

bool Uart::isTxBusy()
{
    HAL_UART_StateTypeDef uart_state = HAL_UART_GetState(&handler);

    return (uart_state == HAL_UART_STATE_BUSY_TX);
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    Uart::RxISR(huart);
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    Uart::TxISR(huart);
}

}  // namespace hardware_layer
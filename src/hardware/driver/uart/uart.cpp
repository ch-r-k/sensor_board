#include "uart.hpp"
#include <cassert>

Uart::Uart(std::uint8_t instance)
{
    assert(instance >= 1 && instance <= 5);

    static constexpr USART_TypeDef *uart_instances[] = {USART1, USART2, USART3,
                                                        UART4, UART5};

    handler.Instance = uart_instances[instance - 1];
}

Uart::~Uart() { handler.Instance = nullptr; }

void Uart::Open()
{
    assert(handler.Instance != nullptr || "nullptr");
    assert(open != true && "must be closed");

    GPIO_InitTypeDef gpioInitStruct = {0};
    RCC_PeriphCLKInitTypeDef periphClkInit = {0};

    gpioInitStruct.Mode = GPIO_MODE_AF_PP;
    gpioInitStruct.Pull = GPIO_NOPULL;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    if (handler.Instance == USART1)
    {
        gpioInitStruct.Pin =
            GPIO_PIN_9 | GPIO_PIN_10;  // Assuming PA9 (TX) and PA10 (RX)
        gpioInitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &gpioInitStruct);
        __HAL_RCC_USART1_CLK_ENABLE();
    }
    else if (handler.Instance == USART2)
    {
        periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
        periphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;

        if (HAL_RCCEx_PeriphCLKConfig(&periphClkInit) != HAL_OK)
        {
            assert(false && "Failed to initialize UART clock");
        }

        /* Peripheral clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpioInitStruct.Pin =
            GPIO_PIN_2 | GPIO_PIN_3;  // Assuming PA2 (TX) and PA3 (RX)
        gpioInitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOA, &gpioInitStruct);
    }
    else if (handler.Instance == USART3)
    {
        gpioInitStruct.Pin =
            GPIO_PIN_10 | GPIO_PIN_11;  // Assuming PB10 (TX) and PB11 (RX)
        gpioInitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOB, &gpioInitStruct);
        __HAL_RCC_USART3_CLK_ENABLE();
    }
    else if (handler.Instance == UART4)
    {
        gpioInitStruct.Pin =
            GPIO_PIN_0 | GPIO_PIN_1;  // Assuming PA0 (TX) and PA1 (RX)
        gpioInitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOA, &gpioInitStruct);
        __HAL_RCC_UART4_CLK_ENABLE();
    }
    else if (handler.Instance == UART5)
    {
        // todo
    }

    if (HAL_UART_Init(&handler) != HAL_OK)
    {
        assert(false && "Failed to initialize UART");
    }
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
        HAL_UART_Transmit(&handler, data.data(), data.size(), 100);

    assert(status == HAL_OK && "UART Write failed");
}

void Uart::StartRead(const std::span<std::uint8_t> data)
{
    assert(open && "UART must be opened before reading");

    HAL_StatusTypeDef status =
        HAL_UART_Receive(&handler, data.data(), data.size(), 100);

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

bool Uart::isBusy()
{
    HAL_UART_StateTypeDef uart_state = HAL_UART_GetState(&handler);

    return (uart_state == HAL_UART_STATE_BUSY);
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    Uart::RxISR(huart);
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    Uart::TxISR(huart);
}
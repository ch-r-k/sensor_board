#include "spi.hpp"
#include <cassert>
#include "driver/interrupt_dispatcher/interrupt.hpp"
#include "stm32l476xx.h"
#include "stm32l4xx_hal_spi.h"
#include "qpcpp.hpp"

Spi::Spi(std::uint8_t instance)
{
    assert(instance >= 1 && instance <= 3);

    static constexpr SPI_TypeDef *spiInstances[] = {SPI1, SPI2, SPI3};

    handler.Instance = spiInstances[instance - 1];

    switch (instance)
    {
        case 1:
            gpioClk.Pin = GPIO_PIN_3;
            gpioMiso.Pin = GPIO_PIN_6;
            gpioMosi.Pin = GPIO_PIN_7;

            portClk = GPIOB;
            portMiso = GPIOA;
            portMosi = GPIOA;

            gpioClk.Alternate = GPIO_AF5_SPI1,
            gpioMiso.Alternate = GPIO_AF5_SPI1;
            gpioMosi.Alternate = GPIO_AF5_SPI1;

            break;

        case 2:
            gpioClk.Pin = GPIO_PIN_10;
            gpioMiso.Pin = GPIO_PIN_2;
            gpioMosi.Pin = GPIO_PIN_3;

            portClk = GPIOB;
            portMiso = GPIOC;
            portMosi = GPIOC;

            gpioClk.Alternate = GPIO_AF5_SPI2,
            gpioMiso.Alternate = GPIO_AF5_SPI2;
            gpioMosi.Alternate = GPIO_AF5_SPI2;

            break;

        case 3:
            gpioClk.Pin = GPIO_PIN_10;
            gpioMiso.Pin = GPIO_PIN_11;
            gpioMosi.Pin = GPIO_PIN_12;

            portClk = GPIOC;
            portMiso = GPIOC;
            portMosi = GPIOC;

            gpioClk.Alternate = GPIO_AF6_SPI3,
            gpioMiso.Alternate = GPIO_AF6_SPI3;
            gpioMosi.Alternate = GPIO_AF6_SPI3;

            break;

        default:
            break;
    }
}
Spi::~Spi() { handler.Instance = nullptr; };

void Spi::Open()
{
    assert((open == false) && "already open");
    assert(handler.Instance != nullptr && "instance can't be nullptr");

    if (handler.Instance == SPI1)
    {
        __HAL_RCC_SPI1_CLK_ENABLE();
    }
    else if (handler.Instance == SPI2)
    {
        __HAL_RCC_SPI2_CLK_ENABLE();
    }
    else if (handler.Instance == SPI3)
    {
        __HAL_RCC_SPI3_CLK_ENABLE();
    }

    gpioClk.Mode = GPIO_MODE_AF_PP;
    gpioClk.Pull = GPIO_PULLUP;
    gpioClk.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    gpioMiso.Mode = GPIO_MODE_AF_PP;
    gpioMiso.Pull = GPIO_PULLUP;
    gpioMiso.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    gpioMosi.Mode = GPIO_MODE_AF_PP;
    gpioMosi.Pull = GPIO_NOPULL;
    gpioMosi.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(portClk, &gpioClk);
    HAL_GPIO_Init(portMiso, &gpioMiso);
    HAL_GPIO_Init(portMosi, &gpioMosi);

    Interrupt &dispatcher = Interrupt::getInstance();

    if (handler.Instance == SPI1)
    {
        dispatcher.registerInterrupt(this, &Spi::Isr, SPI1_IRQn);

        HAL_NVIC_SetPriority(SPI1_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
    else if (handler.Instance == SPI2)
    {
        dispatcher.registerInterrupt(this, &Spi::Isr, SPI2_IRQn);

        HAL_NVIC_SetPriority(SPI2_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }
    else if (handler.Instance == SPI3)
    {
        dispatcher.registerInterrupt(this, &Spi::Isr, SPI3_IRQn);

        HAL_NVIC_SetPriority(SPI3_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(SPI3_IRQn);
    }

    if (HAL_SPI_Init(&handler) != HAL_OK)
    {
        assert(false && "Failed to initialize SPI");
    }

    open = true;
}

void Spi::Close()
{
    HAL_SPI_DeInit(&handler);
    open = false;
}

void Spi::StartWrite(const std::span<const std::uint8_t> data)
{
    assert(open && "must be open");

    if (HAL_SPI_Transmit_IT(&handler, data.data(), data.size()) != HAL_OK)
    {
        assert(false && "Failed to start SPI write");
    }
}

void Spi::StartRead(const std::span<std::uint8_t> data)
{
    assert(open && "must be open");
    HAL_SPI_Receive_IT(&handler, data.data(), data.size());
}

void Spi::Configure(BaudratePrescaler baudratePrescaler)
{
    handler.Init.BaudRatePrescaler =
        static_cast<std::uint32_t>(baudratePrescaler);
}

void Spi::Configure(ClockPhase clockPhase)
{
    handler.Init.CLKPhase = static_cast<std::uint32_t>(clockPhase);
}

void Spi::Configure(ClockPolarity clockPolarity)
{
    handler.Init.CLKPolarity = static_cast<std::uint32_t>(clockPolarity);
}

void Spi::Configure(CRCCalculation crcCalculation)
{
    handler.Init.CRCCalculation = static_cast<std::uint32_t>(crcCalculation);
}

void Spi::Configure(DataSize dataSize)
{
    handler.Init.DataSize = static_cast<std::uint32_t>(dataSize);
}

void Spi::Configure(Direction direction)
{
    handler.Init.Direction = static_cast<std::uint32_t>(direction);
}

void Spi::Configure(FirstBit firstBit)
{
    handler.Init.FirstBit = static_cast<std::uint32_t>(firstBit);
}

void Spi::Configure(Mode mode)
{
    handler.Init.Mode = static_cast<std::uint32_t>(mode);
}

void Spi::Configure(Nss nss)
{
    handler.Init.NSS = static_cast<std::uint32_t>(nss);
}

void Spi::Configure(NssPMode nssPMode)
{
    handler.Init.NSSPMode = static_cast<std::uint32_t>(nssPMode);
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    Spi::RxISR(hspi);
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    Spi::TxISR(hspi);
}

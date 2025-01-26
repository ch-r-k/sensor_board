#include "spi.hpp"
#include <cassert>
#include "driver/interrupt_dispatcher/interrupt.hpp"
#include "stm32l476xx.h"
#include "stm32l4xx_hal_spi.h"
#include "qpcpp.hpp"

namespace hardware_layer
{
Spi::Spi(std::uint8_t instance)
{
    assert(instance >= 1 && instance <= 3);

    static constexpr SPI_TypeDef *SPI_INSTANCES[] = {SPI1, SPI2, SPI3};

    handler.Instance = SPI_INSTANCES[instance - 1];

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

void Spi::open()
{
    assert((isOpen == false) && "already open");
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
        dispatcher.registerInterrupt(this, &Spi::isr, SPI1_IRQn);

        HAL_NVIC_SetPriority(SPI1_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
    else if (handler.Instance == SPI2)
    {
        dispatcher.registerInterrupt(this, &Spi::isr, SPI2_IRQn);

        HAL_NVIC_SetPriority(SPI2_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }
    else if (handler.Instance == SPI3)
    {
        dispatcher.registerInterrupt(this, &Spi::isr, SPI3_IRQn);

        HAL_NVIC_SetPriority(SPI3_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1, 0);
        HAL_NVIC_EnableIRQ(SPI3_IRQn);
    }

    if (HAL_SPI_Init(&handler) != HAL_OK)
    {
        assert(false && "Failed to initialize SPI");
    }

    isOpen = true;
}

void Spi::close()
{
    HAL_SPI_DeInit(&handler);
    isOpen = false;
}

void Spi::startWrite(const std::span<const std::uint8_t> data)
{
    assert(isOpen && "must be open");

    if (HAL_SPI_Transmit_IT(&handler, data.data(), data.size()) != HAL_OK)
    {
        assert(false && "Failed to start SPI write");
    }
}

void Spi::startRead(const std::span<std::uint8_t> data)
{
    assert(isOpen && "must be open");
    HAL_SPI_Receive_IT(&handler, data.data(), data.size());
}

void Spi::configure(BaudratePrescaler baudrate_prescaler)
{
    handler.Init.BaudRatePrescaler =
        static_cast<std::uint32_t>(baudrate_prescaler);
}

void Spi::configure(ClockPhase clock_phase)
{
    handler.Init.CLKPhase = static_cast<std::uint32_t>(clock_phase);
}

void Spi::configure(ClockPolarity clock_polarity)
{
    handler.Init.CLKPolarity = static_cast<std::uint32_t>(clock_polarity);
}

void Spi::configure(CRCCalculation crc_calculation)
{
    handler.Init.CRCCalculation = static_cast<std::uint32_t>(crc_calculation);
}

void Spi::configure(DataSize data_size)
{
    handler.Init.DataSize = static_cast<std::uint32_t>(data_size);
}

void Spi::configure(Direction direction)
{
    handler.Init.Direction = static_cast<std::uint32_t>(direction);
}

void Spi::configure(FirstBit first_bit)
{
    handler.Init.FirstBit = static_cast<std::uint32_t>(first_bit);
}

void Spi::configure(Mode mode)
{
    handler.Init.Mode = static_cast<std::uint32_t>(mode);
}

void Spi::configure(Nss nss)
{
    handler.Init.NSS = static_cast<std::uint32_t>(nss);
}

void Spi::configure(NssPMode nss_p_mode)
{
    handler.Init.NSSPMode = static_cast<std::uint32_t>(nss_p_mode);
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    Spi::rxIsr(hspi);
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    Spi::txIsr(hspi);
}

}  // namespace hardware_layer
#include "spi.hpp"
#include <cassert>

Spi::Spi() { handler.Instance = SPI1; }
Spi::~Spi() { handler.Instance = nullptr; };

void Spi::Open()
{
    assert(open == false || "already open");
    assert(handler.Instance != nullptr || "instance can't be nullptr");

    open = true;
    HAL_SPI_Init(&handler);
}

void Spi::Close()
{
    HAL_SPI_DeInit(&handler);
    open = false;
}

void Spi::StartWrite(const std::span<const std::uint8_t> data)
{
    assert(open || "must be open");
    HAL_SPI_Transmit_IT(&handler, data.data(), data.size());
}

void Spi::StartRead(const std::span<std::uint8_t> data)
{
    assert(open || "must be open");
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

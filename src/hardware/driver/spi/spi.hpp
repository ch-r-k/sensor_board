#ifndef SPI_HPP
#define SPI_HPP

#include "i_spi.hpp"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include <cassert>

class Spi : public ISpi
{
   public:
    enum class BaudratePrescaler : std::uint8_t
    {
        preScale2 = SPI_BAUDRATEPRESCALER_2,
        preScale4 = SPI_BAUDRATEPRESCALER_4,
        preScale8 = SPI_BAUDRATEPRESCALER_8,
        preScale16 = SPI_BAUDRATEPRESCALER_16,
        preScale32 = SPI_BAUDRATEPRESCALER_32,
        preScale64 = SPI_BAUDRATEPRESCALER_64,
        preScale128 = SPI_BAUDRATEPRESCALER_128,
        preScale256 = SPI_BAUDRATEPRESCALER_256,
    };

    enum class ClockPhase : std::uint8_t
    {
        Phase1Edge = SPI_PHASE_1EDGE,
        Phase2Edge = SPI_PHASE_2EDGE
    };

    enum class ClockPolarity : std::uint8_t
    {
        PolarityLow = SPI_POLARITY_LOW,
        PolarityHigh = SPI_POLARITY_HIGH
    };

    enum class CRCCalculation : std::uint16_t
    {
        Disable = SPI_CRCCALCULATION_DISABLE,
        Enable = SPI_CRCCALCULATION_ENABLE
    };

    enum class DataSize : std::uint16_t
    {
        DataSize8Bit = SPI_DATASIZE_8BIT,
        DataSize16Bit = SPI_DATASIZE_16BIT
    };

    enum class Direction : std::uint16_t
    {
        Direction2Lines = SPI_DIRECTION_2LINES,
        Direction1Line = SPI_DIRECTION_1LINE
    };

    enum class FirstBit : std::uint8_t
    {
        MSB = SPI_FIRSTBIT_MSB,
        LSB = SPI_FIRSTBIT_LSB
    };

    enum class Mode : std::uint16_t
    {
        Master = SPI_MODE_MASTER,
        Slave = SPI_MODE_SLAVE
    };

    enum class Nss : std::uint32_t
    {
        HardOutput = SPI_NSS_HARD_OUTPUT,
        Soft = SPI_NSS_SOFT,
        HardInput = SPI_NSS_HARD_INPUT
    };

    enum class NssPMode : uint8_t
    {
        PulseEnable = SPI_NSS_PULSE_ENABLE,
        PulseDisable = SPI_NSS_PULSE_DISABLE
    };

   private:
    SPI_HandleTypeDef handler;
    bool open = false;

   public:
    Spi();
    ~Spi();

    void Open() override;

    void Close() override;

    void StartWrite(const std::span<const std::uint8_t> data) override;

    void StartRead(const std::span<std::uint8_t> data) override;

    void Configure(BaudratePrescaler baudratePrescaler);
    void Configure(ClockPhase clockPhase);
    void Configure(ClockPolarity clockPolarity);
    void Configure(CRCCalculation crcCalculation);
    void Configure(DataSize dataSize);
    void Configure(Direction direction);
    void Configure(FirstBit firstBit);
    void Configure(Mode mode);
    void Configure(Nss nss);
    void Configure(NssPMode nssPMode);

    static void RxISR(SPI_HandleTypeDef *hspi)
    {
        if (hspi->Instance == SPI1)
        {
        }
        else
        {
            assert(false || "not implemented");
        }
    }

    static void TxISR(SPI_HandleTypeDef *hspi)
    {
        if (hspi->Instance == SPI1)
        {
        }
        else
        {
            assert(false || "not implemented");
        }
    }
};

#endif  // SPI_HPP
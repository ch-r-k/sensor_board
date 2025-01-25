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
        PRE_SCALE2 = SPI_BAUDRATEPRESCALER_2,
        PRE_SCALE4 = SPI_BAUDRATEPRESCALER_4,
        PRE_SCALE8 = SPI_BAUDRATEPRESCALER_8,
        PRE_SCALE16 = SPI_BAUDRATEPRESCALER_16,
        PRE_SCALE32 = SPI_BAUDRATEPRESCALER_32,
        PRE_SCALE64 = SPI_BAUDRATEPRESCALER_64,
        PRE_SCALE128 = SPI_BAUDRATEPRESCALER_128,
        PRE_SCALE256 = SPI_BAUDRATEPRESCALER_256,
    };

    enum class ClockPhase : std::uint8_t
    {
        PHASE1_EDGE = SPI_PHASE_1EDGE,
        PHASE2_EDGE = SPI_PHASE_2EDGE
    };

    enum class ClockPolarity : std::uint8_t
    {
        POLARITY_LOW = SPI_POLARITY_LOW,
        POLARITY_HIGH = SPI_POLARITY_HIGH
    };

    enum class CRCCalculation : std::uint16_t
    {
        DISABLE = SPI_CRCCALCULATION_DISABLE,
        ENABLE = SPI_CRCCALCULATION_ENABLE
    };

    enum class DataSize : std::uint16_t
    {
        DATA_SIZE8_BIT = SPI_DATASIZE_8BIT,
        DATA_SIZE16_BIT = SPI_DATASIZE_16BIT
    };

    enum class Direction : std::uint16_t
    {
        DIRECTION2_LINES = SPI_DIRECTION_2LINES,
        DIRECTION1_LINE = SPI_DIRECTION_1LINE
    };

    enum class FirstBit : std::uint8_t
    {
        MSB = SPI_FIRSTBIT_MSB,
        LSB = SPI_FIRSTBIT_LSB
    };

    enum class Mode : std::uint16_t
    {
        MASTER = SPI_MODE_MASTER,
        SLAVE = SPI_MODE_SLAVE
    };

    enum class Nss : std::uint32_t
    {
        HARD_OUTPUT = SPI_NSS_HARD_OUTPUT,
        SOFT = SPI_NSS_SOFT,
        HARD_INPUT = SPI_NSS_HARD_INPUT
    };

    enum class NssPMode : uint8_t
    {
        PULSE_ENABLE = SPI_NSS_PULSE_ENABLE,
        PULSE_DISABLE = SPI_NSS_PULSE_DISABLE
    };

   private:
    SPI_HandleTypeDef handler;
    bool isOpen = false;

    GPIO_InitTypeDef gpioClk = {};
    GPIO_TypeDef* portClk = nullptr;

    GPIO_InitTypeDef gpioMiso = {};
    GPIO_TypeDef* portMiso = nullptr;

    GPIO_InitTypeDef gpioMosi = {};
    GPIO_TypeDef* portMosi = nullptr;

   public:
    Spi(std::uint8_t instance);
    ~Spi();

    void open() override;

    void close() override;

    void startWrite(const std::span<const std::uint8_t> data) override;

    void startRead(const std::span<std::uint8_t> data) override;

    void configure(BaudratePrescaler baudrate_prescaler);
    void configure(ClockPhase clock_phase);
    void configure(ClockPolarity clock_polarity);
    void configure(CRCCalculation crc_calculation);
    void configure(DataSize data_size);
    void configure(Direction direction);
    void configure(FirstBit first_bit);
    void configure(Mode mode);
    void configure(Nss nss);
    void configure(NssPMode nss_p_mode);

    static void isr(void* callback_object, [[maybe_unused]] void* parameter)
    {
        // Cast callbackObject to Spi* and call the non-static ISR
        Spi* spi = static_cast<Spi*>(callback_object);

        HAL_SPI_IRQHandler(&spi->handler);
    }

    static void rxIsr(SPI_HandleTypeDef* hspi)
    {
        if (hspi->Instance == SPI2)
        {
        }
        else
        {
            assert(false && "not implemented");
        }
    }

    static void txIsr(SPI_HandleTypeDef* hspi)
    {
        if (hspi->Instance == SPI2)
        {
        }
        else
        {
            assert(false && "not implemented");
        }
    }
};

#endif  // SPI_HPP
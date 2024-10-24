#ifndef OUTPUT_PIN_HPP
#define OUTPUT_PIN_HPP

#include <cstdint>

#include "i_output_pin.hpp"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"

class OutputPin : public IOutputPin
{
   public:
    enum class Port : uint8_t
    {
        PORT_A,
        PORT_B,
        PORT_C,
        PORT_D,
        PORT_E,
        PORT_F,
        PORT_G,
        PORT_H,
    };

    enum class Pin : uint16_t
    {
        PIN_0 = GPIO_PIN_0,
        PIN_1 = GPIO_PIN_1,
        PIN_2 = GPIO_PIN_2,
        PIN_3 = GPIO_PIN_3,
        PIN_4 = GPIO_PIN_4,
        PIN_5 = GPIO_PIN_5,
        PIN_6 = GPIO_PIN_6,
        PIN_7 = GPIO_PIN_7,
        PIN_8 = GPIO_PIN_8,
        PIN_9 = GPIO_PIN_9,
        PIN_10 = GPIO_PIN_10,
        PIN_11 = GPIO_PIN_11,
        PIN_12 = GPIO_PIN_12,
        PIN_13 = GPIO_PIN_13,
        PIN_14 = GPIO_PIN_14,
        PIN_15 = GPIO_PIN_15,
    };

    enum class Mode : uint8_t
    {
        PUSH_PULL = GPIO_MODE_OUTPUT_PP,
        OPEN_DRAIN = GPIO_MODE_OUTPUT_OD
    };

    enum class Pull : uint8_t
    {
        NOPULL = GPIO_NOPULL,
        PULL_UP = GPIO_PULLUP,
        PULL_DOWN = GPIO_PULLDOWN
    };

    enum class Speed : uint8_t
    {
        LOW = 0,
        MEDIUM,
        HIGH,
        VERY_HIGH
    };

   private:
    Port port;
    Pin pin;
    Mode mode;
    Pull pull;
    Speed speed;

   public:
    OutputPin();
    ~OutputPin();
    void configure(Port initPort, Pin initPin, Mode initMode, Pull initPull,
                   Speed initSpeed);
    void set() override;
    void reset() override;
    GPIO_TypeDef* getPort(Port port);
};

#endif  // OUTPUT_PIN_HPP
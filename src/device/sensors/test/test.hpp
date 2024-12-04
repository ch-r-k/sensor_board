#ifndef TEST_HPP
#define TEST_HPP

#include "../i_sensor.hpp"
#include "driver/i2c/i_i2c.hpp"
#include "driver/spi/i_spi.hpp"
#include <cassert>

class Test : public ISensor
{
   public:
   private:
    bool open = false;
    ISpi* iSpi = nullptr;
    II2c* iI2c = nullptr;

   public:
    Test();
    ~Test();

    void Open() override;
    void Close() override;
    void StartSensor() override;
    void setSpiInterface(ISpi& i_spi);
    void setI2cInterface(II2c& i_i2c);
};

#endif  // TEST_HPP
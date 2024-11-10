#ifndef TEST_HPP
#define TEST_HPP

#include "../i_sensor.hpp"
#include "driver/spi/i_spi.hpp"
#include <cassert>

class Test : public ISensor
{
   public:
   private:
    bool open = false;
    ISpi* iSpi = nullptr;

   public:
    Test();
    ~Test();

    void Open() override;
    void Close() override;
    void StartSensor() override;
    void setSpiInterface(ISpi& i_spi);
};

#endif  // TEST_HPP
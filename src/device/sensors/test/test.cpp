#include "test.hpp"
#include <cassert>
#include <cstdint>

Test::Test() {}
Test::~Test() {}

void Test::open()
{
    assert(isOpen == false && "already open");
    assert(iSpi != nullptr && "instance can't be nullptr");

    isOpen = true;
    iSpi->open();
    iI2c->open();
}

void Test::close() { isOpen = false; }

void Test::triggerMeasurement()
{
    assert(isOpen && "must be open");

    const std::array<const std::uint8_t, 3> temp = {
        std::uint8_t{1}, std::uint8_t{1}, std::uint8_t{1}};
    iSpi->startWrite(temp);
    iI2c->startWrite(temp);
}

void Test::setSpiInterface(ISpi& i_spi) { iSpi = &i_spi; }

void Test::setI2cInterface(II2c& i_i2c) { iI2c = &i_i2c; }
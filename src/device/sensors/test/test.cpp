#include "test.hpp"
#include <cassert>
#include <cstdint>

Test::Test() {}
Test::~Test() {}

void Test::Open()
{
    assert(open == false && "already open");
    assert(iSpi != nullptr && "instance can't be nullptr");

    open = true;
    iSpi->Open();
}

void Test::Close() { open = false; }

void Test::StartSensor()
{
    assert(open && "must be open");

    const std::array<const std::uint8_t, 3> temp = {
        std::uint8_t{1}, std::uint8_t{1}, std::uint8_t{1}};
    iSpi->StartWrite(temp);
}

void Test::setSpiInterface(ISpi& i_spi) { iSpi = &i_spi; }
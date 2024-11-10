#include "test.hpp"
#include <cassert>

Test::Test() {}
Test::~Test() {}

void Test::Open()
{
    assert(open == false || "already open");
    assert(iSpi != nullptr || "instance can't be nullptr");

    open = true;
    iSpi->Open();

    const std::array<const std::byte, 3> temp = {std::byte{1}, std::byte{1},
                                                 std::byte{1}};
    // iSpi->StartWrite(temp);
}

void Test::Close() { open = false; }

void Test::StartSensor() { assert(open || "must be open"); }

void Test::setSpiInterface(ISpi& i_spi) { iSpi = &i_spi; }
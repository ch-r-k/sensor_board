#include "ssd1306.hpp"
#include <bits/c++config.h>
#include <cassert>
#include <cstdint>
#include "serial_commander/serial_commander.hpp"
#include "stm32l4xx_hal_def.h"

namespace device_layer
{
Ssd1306::Ssd1306() {}
Ssd1306::~Ssd1306() {}

void Ssd1306::open()
{
    assert(!isOpen && "already open");
    isOpen = true;
    init();
}

void Ssd1306::close()
{
    isOpen = false;
    initialized = false;  // Reset initialization state
}

void Ssd1306::init()
{
    assert(isOpen && "must be open");

    ISerialCommander::Command command;

    prepareCommand(command,
                   {0, static_cast<std::uint8_t>(Command::SOFT_RESET)});
    iSerial->setCommand(command);

    displayOff(command);
    iSerial->setCommand(command);

    setMuxRatio(command);
    iSerial->setCommand(command);

    memoryAddrMode(command);
    iSerial->setCommand(command);

    setStartLine(command);
    iSerial->setCommand(command);

    displayOffset(command);
    iSerial->setCommand(command);

    segRemapOp(command);
    iSerial->setCommand(command);

    comScanDirOp(command);
    iSerial->setCommand(command);

    // Com Pins Hardware configuration
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_COM_PINS_CONFIG), 0x12});
    iSerial->setCommand(command);

    setContrast(command);
    iSerial->setCommand(command);

    entireDisplayOn(command);
    iSerial->setCommand(command);

    setDisplayNormal(command);
    iSerial->setCommand(command);

    setOscFreq(command);
    iSerial->setCommand(command);

    enableChargePump(command);
    iSerial->setCommand(command);

    // deactivate Scroll
    prepareCommand(command,
                   {0, static_cast<std::uint8_t>(Command::DEACTIVATE_SCROLL)});
    iSerial->setCommand(command);

    displayOn(command);
    iSerial->setCommand(command);
    // -----------------------------

    iSerial->setI2CAddress(address);
    iSerial->startCommands();

    initialized = true;
}

void Ssd1306::clear() { cache.fill(0U); }

void Ssd1306::drawPixel(std::uint32_t x, std::uint32_t y)
{
    std::uint8_t page = 0;
    std::uint8_t pixel = 0;

    assert((x <= MAX_X) && (y <= MAX_Y));

    page = y / RAM_Y_END;   // find page (y / 8)
    pixel = y % RAM_Y_END;  // which pixel (y % 8)

    std::size_t index = x + (page << 7);
    cache[index + 1] = 1U << pixel;  // save pixel
}

void Ssd1306::update()
{
    assert(isOpen && "must be open");
    assert(initialized && "must be initialized");

    ISerialCommander::Command command;

    // set command
    cache[0] = 0x40;

    // write cache
    command.instruction = Instructions::WRITE_SPAN;
    command.data_span = cache;
    iSerial->setCommand(command);

    iSerial->setI2CAddress(address);
    iSerial->startCommands();
}

void Ssd1306::setSerialCommanderInterface(ISerialCommander& i_serial_commander)
{
    iSerial = &i_serial_commander;
}

void Ssd1306::prepareCommand(ISerialCommander::Command& command,
                             const std::initializer_list<std::uint8_t>& data)
{
    command.instruction = ISerialCommander::Instructions::WRITE;
    std::copy(data.begin(), data.end(), command.data.begin());
    command.data_length = data.size();
}

void Ssd1306::displayOff(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {0, static_cast<std::uint8_t>(Command::SET_DISPLAY)});
}

void Ssd1306::setOscFreq(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_CLOCK_DIVIDER), 0x80});
}

void Ssd1306::setMuxRatio(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_MULTIPLEX_RATIO), 0x3F});
}

void Ssd1306::displayOffset(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_DISPLAY_OFFSET), 0x00});
}

void Ssd1306::setStartLine(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_DISPLAY_START_LINE)});
}

void Ssd1306::memoryAddrMode(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_MEMORY_MODE), 0x00});
}

void Ssd1306::segRemapOp(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_SEGMENT_REMAP) | 0x01});
}

void Ssd1306::comScanDirOp(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_COM_OUTPUT) | 0x00});
}

void Ssd1306::setContrast(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::SET_CONTRAST_CONTROL), 0x7F});
}

void Ssd1306::displayOn(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {0, static_cast<std::uint8_t>(Command::SET_DISPLAY) | 0x01});
}
void Ssd1306::entireDisplayOn(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {0, static_cast<std::uint8_t>(Command::ENTRY_DISPLAY_ON)});
}
void Ssd1306::enableChargePump(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {0, static_cast<std::uint8_t>(Command::ENABLE_CHARGE_PUMP), 0x14});
}

void Ssd1306::setDisplayNormal(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {0, static_cast<std::uint8_t>(Command::DISPLAY_NORMAL)});
}

void Ssd1306::done(IcbSerialCommander::ReturnValue return_value)
{
    UNUSED(return_value);
    // todo: implement
    icbDisplay->initDone();
    // icbDisplay->printDone();
}

void Ssd1306::setIcbDisplay(IcbDisplay& icb_display)
{
    icbDisplay = &icb_display;
}
}  // namespace device_layer
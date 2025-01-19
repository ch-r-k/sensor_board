#include "ssd1306.hpp"
#include <cassert>
#include <cstdint>
#include "serial_commander/serial_commander.hpp"
#include "stm32l4xx_hal_def.h"

Ssd1306::Ssd1306() {}
Ssd1306::~Ssd1306() {}

void Ssd1306::Open()
{
    assert(!open && "already open");
    Init();
    open = true;
}

void Ssd1306::Close()
{
    open = false;
    initialized = false;  // Reset initialization state
}

void Ssd1306::Init()
{
    assert(open && "must be open");

    ISerialCommander::Command command;

    displayOff(command);
    iSerial->SetCommand(command);

    setOscFreq(command);
    iSerial->SetCommand(command);

    setMuxRatio(command);
    iSerial->SetCommand(command);

    displayOffset(command);
    iSerial->SetCommand(command);

    setStartLine(command);
    iSerial->SetCommand(command);

    memoryAddrMode(command);
    iSerial->SetCommand(command);

    segRemapOp(command);
    iSerial->SetCommand(command);

    comScanDirOp(command);
    iSerial->SetCommand(command);

    setContrast(command);
    iSerial->SetCommand(command);

    displayOn(command);
    iSerial->SetCommand(command);

    iSerial->SetI2CAddress(address);
    iSerial->StartCommands();

    initialized = true;
}

void Ssd1306::clear() { cache.fill(0U); }

void Ssd1306::drawPixel(uint8_t x, uint8_t y)
{
    std::uint8_t page = 0;
    std::uint8_t pixel = 0;

    assert((x <= maxX) && (y <= maxY));

    page = y >> endPageAddress;                   // find page (y / 8)
    pixel = 1 << (y - (page << endPageAddress));  // which pixel (y % 8)
    cache[x + (page << 7)] |= pixel;              // save pixel
}

void Ssd1306::update()
{
    assert(open && "must be open");
    assert(initialized && "must be initialized");

    ISerialCommander::Command command;

    // set start line (in this case 0)
    command.instruction = Instructions::Write;
    command.data[0] =
        static_cast<std::uint8_t>(Command::SET_DISPLAY_START_LINE);
    command.data_length = 1;
    iSerial->SetCommand(command);

    // write cache
    command.instruction = Instructions::WriteSpan;
    command.data_span = cache;
    iSerial->SetCommand(command);

    iSerial->SetI2CAddress(address);
    iSerial->StartCommands();
}

void Ssd1306::setSerialCommanderInterface(ISerialCommander& i_serial_commander)
{
    iSerial = &i_serial_commander;
}

void Ssd1306::prepareCommand(ISerialCommander::Command& command,
                             const std::initializer_list<std::uint8_t>& data)
{
    command.instruction = ISerialCommander::Instructions::Write;
    std::copy(data.begin(), data.end(), command.data.begin());
    command.data_length = data.size();
}

void Ssd1306::displayOff(ISerialCommander::Command& command)
{
    prepareCommand(command, {static_cast<std::uint8_t>(Command::SET_DISPLAY)});
}

void Ssd1306::setOscFreq(ISerialCommander::Command& command)
{
    prepareCommand(
        command, {static_cast<std::uint8_t>(Command::SET_CLOCK_DIVIDER), 0x80});
}

void Ssd1306::setMuxRatio(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {static_cast<std::uint8_t>(Command::SET_MULTIPLEX_RATIO), 0x3F});
}

void Ssd1306::displayOffset(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {static_cast<std::uint8_t>(Command::SET_DISPLAY_OFF), 0x00});
}

void Ssd1306::setStartLine(ISerialCommander::Command& command)
{
    prepareCommand(
        command, {static_cast<std::uint8_t>(Command::SET_DISPLAY_START_LINE)});
}

void Ssd1306::memoryAddrMode(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {static_cast<std::uint8_t>(Command::SET_MEMORY_MODE), 0x00});
}

void Ssd1306::segRemapOp(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {static_cast<std::uint8_t>(Command::SET_SEGMENT_REMAP) | 0x01});
}

void Ssd1306::comScanDirOp(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {static_cast<std::uint8_t>(Command::SET_COM_OUTPUT) | 0x08});
}

void Ssd1306::setContrast(ISerialCommander::Command& command)
{
    prepareCommand(
        command,
        {static_cast<std::uint8_t>(Command::SET_CONTRAST_CONTROL), 0x7F});
}

void Ssd1306::displayOn(ISerialCommander::Command& command)
{
    prepareCommand(command,
                   {static_cast<std::uint8_t>(Command::SET_DISPLAY) | 0x01});
}

void Ssd1306::Done(IcbSerialCommander::ReturnValue return_value)
{
    UNUSED(return_value);
    // todo: implement
}
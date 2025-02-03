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

    prepareCommand(command, Command::SET_DISPLAY, 0);
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_MULTIPLEX_RATIO, 0, {0x3F});
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_MEMORY_MODE, 0, {0x00});
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_DISPLAY_START_LINE, 0);
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_DISPLAY_OFFSET, 0, {0x00});
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_SEGMENT_REMAP, 0x01);
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_COM_OUTPUT, 0x00);
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_COM_PINS_CONFIG, 0x00, {0x12});
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_CONTRAST_CONTROL, 0x00, {0x7F});
    iSerial->setCommand(command);

    prepareCommand(command, Command::ENTRY_DISPLAY_ON, 0x00);
    iSerial->setCommand(command);

    prepareCommand(command, Command::DISPLAY_NORMAL, 0x00);
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_CLOCK_DIVIDER, 0x00, {0x80});
    iSerial->setCommand(command);

    prepareCommand(command, Command::ENABLE_CHARGE_PUMP, 0x00, {0x14});
    iSerial->setCommand(command);

    prepareCommand(command, Command::DEACTIVATE_SCROLL, 0x00);
    iSerial->setCommand(command);

    prepareCommand(command, Command::SET_DISPLAY, 0x01);
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
    cache[0] = static_cast<std::uint8_t>(Command::SET_DISPLAY_START_LINE);

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

void Ssd1306::prepareCommand(ISerialCommander::Command& serial_command,
                             Command device_command,
                             std::uint8_t device_command_manipulator,
                             const std::initializer_list<std::uint8_t>& data)
{
    serial_command.instruction = ISerialCommander::Instructions::WRITE;
    serial_command.data[0] = 0;

    serial_command.data[1] =
        static_cast<std::uint8_t>(device_command) | device_command_manipulator;

    std::copy(data.begin(), data.end(), serial_command.data.begin() + 2);

    serial_command.data_length = data.size() + 2;
}

void Ssd1306::prepareCommand(ISerialCommander::Command& serial_command,
                             Command device_command,
                             std::uint8_t device_command_manipulator)
{
    serial_command.instruction = ISerialCommander::Instructions::WRITE;
    serial_command.data[0] = 0;

    serial_command.data[1] =
        static_cast<std::uint8_t>(device_command) | device_command_manipulator;

    serial_command.data_length = 2;
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
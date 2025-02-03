#ifndef SSD1306_HPP
#define SSD1306_HPP

#include <bits/c++config.h>
#include <array>
#include <cassert>
#include <cstdint>
#include <iterator>

#include "../i_display.hpp"
#include "../icb_display.hpp"

#include "serial_commander/i_serial_commander.hpp"
#include "serial_commander/icb_serial_commander.hpp"

namespace device_layer
{
class Ssd1306 : public IDisplay, public IcbSerialCommander
{
   public:
    // Enum for Ssd1306 commands
    enum class Command : std::uint8_t
    {
        // Fundament Commands
        SOFT_RESET = 0xE4,   // 1 byte
        SET_DISPLAY = 0xAE,  // 1 byte

        ENTRY_DISPLAY_ON = 0xA4,  // 1 byte

        DISPLAY_NORMAL = 0xA6,    // 1 byte
        DISPLAY_INVERTED = 0xA7,  // 1 byte

        SET_CONTRAST_CONTROL = 0x81,  // 2 byte

        // Scrolling Commands
        RIGHT_HORIZONTAL_SCROLL = 0x26,  // 7 byte
        LEFT_HORIZONTAL_SCROLL = 0x27,   // 7 byte

        VERTICAL_AND_RIGHT_SCROLL = 0x29,  // 7 byte
        VERTICAL_AND_LEFT_SCROLL = 0x2A,   // 7 byte

        DEACTIVATE_SCROLL = 0x2E,  // 1 byte
        ACTIVATE_SCROLL = 0x2F,    // 1 byte

        VERTICAL_SCROLL = 0xA3,  // 2 byte

        // Address Setting Commands
        SET_LOWER_COLUMN = 0x00,    // 2 byte
        SET_HIGHER_COLUMN = 0x01,   // 2 byte
        SET_MEMORY_MODE = 0x20,     // 2 byte
        SET_COLUMN_ADDRESS = 0x21,  // 3 byte
        SET_PAGE_ADDRESS = 0x22,    // 3 byte
        SET_PAGE_START = 0xB0,      // 1 byte

        // Hardware Configuration Command
        SET_DISPLAY_START_LINE = 0x40,  // 1 byte
        SET_SEGMENT_REMAP = 0xA0,       // 1 byte
        SET_MULTIPLEX_RATIO = 0xA8,     // 2 byte
        SET_COM_OUTPUT = 0xC0,          // 1 byte
        SET_DISPLAY_OFFSET = 0xD3,      // 2 byte
        SET_COM_PINS_CONFIG = 0xDA,     // 2 byte

        // Timing
        SET_CLOCK_DIVIDER = 0xD5,         // 2 byte
        SET_PRECHARGE_PERIOD = 0xD9,      // 2 byte
        SET_VCOMH_DESELECT_LEVEL = 0xDB,  // 2 byte

        // Charge pump
        ENABLE_CHARGE_PUMP = 0x8D,
    };

   private:
    static constexpr std::size_t START_PAGE_ADDRESS = 0;
    static constexpr std::size_t END_PAGE_ADDRESS = 7;
    static constexpr std::size_t START_COLUMN_NUMBER = 0;
    static constexpr std::size_t END_COLUMN_ADDRESS = 127;
    static constexpr std::size_t RAM_X_END = END_COLUMN_ADDRESS + 1;
    static constexpr std::size_t RAM_Y_END = END_PAGE_ADDRESS + 1;
    static constexpr std::size_t CACHE_SIZE = (RAM_X_END) * (RAM_Y_END);

    static constexpr std::size_t MAX_X = END_COLUMN_ADDRESS;
    static constexpr std::size_t MAX_Y = (END_PAGE_ADDRESS + 1) * 8;

   private:
    bool isOpen = false;
    bool initialized = false;

    IcbDisplay* icbDisplay = nullptr;
    ISerialCommander* iSerial = nullptr;

    std::uint8_t address = 0x3C;
    std::array<std::uint8_t, 6> readBuffer = {0};
    std::array<std::uint8_t, CACHE_SIZE + 1> cache;

   public:
    Ssd1306();
    ~Ssd1306();

    void open() override;
    void close() override;

    void done(IcbSerialCommander::ReturnValue return_value) override;

    void init();
    void drawPixel(std::uint32_t x, std::uint32_t y) override;
    void clear() override;
    void update() override;

    void setSerialCommanderInterface(ISerialCommander& i_serial_commander);
    void setIcbDisplay(IcbDisplay& icb_display);

   private:
    void prepareCommand(ISerialCommander::Command& serial_command,
                        Command device_command,
                        std::uint8_t device_command_manipulator,
                        const std::initializer_list<std::uint8_t>& data);

    void prepareCommand(ISerialCommander::Command& serial_command,
                        Command device_command,
                        std::uint8_t device_command_manipulator);

    void displayOff(ISerialCommander::Command& command);
    void setOscFreq(ISerialCommander::Command& command);
    void setMuxRatio(ISerialCommander::Command& command);
    void displayOffset(ISerialCommander::Command& command);
    void setStartLine(ISerialCommander::Command& command);
    void setCharReg(ISerialCommander::Command& command);
    void memoryAddrMode(ISerialCommander::Command& command);
    void segRemapOp(ISerialCommander::Command& command);
    void comScanDirOp(ISerialCommander::Command& command);
    void comPinConf(ISerialCommander::Command& command);
    void setContrast(ISerialCommander::Command& command);
    void setPrecharge(ISerialCommander::Command& command);
    void vcomDeselect(ISerialCommander::Command& command);
    void disEntDispOn(ISerialCommander::Command& command);
    void disNormal(ISerialCommander::Command& command);
    void deactScroll(ISerialCommander::Command& command);
    void entireDisplayOn(ISerialCommander::Command& command);
    void enableChargePump(ISerialCommander::Command& command);
    void setDisplayNormal(ISerialCommander::Command& command);
    void displayOn(ISerialCommander::Command& command);
};

}  // namespace device_layer
#endif  // SSD1306_HPP

#ifndef I_SERIAL_COMMANDER_HPP
#define I_SERIAL_COMMANDER_HPP

#include <cstdint>
#include <array>
#include <span>

class ISerialCommander
{
   public:
    enum class Instructions : std::uint8_t
    {
        Write,
        WriteSpan,  // don't change during writing
        Read,
        Pause
    };

    struct Command
    {
        Instructions instruction;
        std::array<std::uint8_t, 16U> data;
        std::size_t data_length;
        std::span<std::uint8_t> data_span;
        std::uint16_t pauseTime;
    };

   private:
   public:
    virtual ~ISerialCommander() = default;
    virtual void SetCommand(Command command) = 0;
    virtual void StartCommands() = 0;
};

#endif  // I_SERIAL_COMMANDER_HPP
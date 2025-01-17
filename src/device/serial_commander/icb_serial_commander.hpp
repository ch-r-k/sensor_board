#ifndef ICB_SERIAL_COMMANDER_HPP
#define ICB_SERIAL_COMMANDER_HPP

#include <array>
#include <cstdint>
class IcbSerialCommander
{
   public:
    struct ReturnValue
    {
        std::array<std::uint8_t, 16U> data;
        std::size_t data_length;
    };

   private:
   public:
    virtual ~IcbSerialCommander() = default;
    virtual void Done(ReturnValue return_value) = 0;
};

#endif  // ICB_SERIAL_COMMANDER_HPP
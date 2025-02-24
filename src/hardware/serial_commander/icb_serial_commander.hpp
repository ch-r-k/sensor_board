#ifndef ICB_SERIAL_COMMANDER_HPP
#define ICB_SERIAL_COMMANDER_HPP

#include <array>
#include <cstdint>

namespace hardware_layer
{

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
    virtual void done(ReturnValue return_value) = 0;
};

}  // namespace hardware_layer

#endif  // ICB_SERIAL_COMMANDER_HPP
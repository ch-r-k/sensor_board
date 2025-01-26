#ifndef I_DISPLAY_HPP
#define I_DISPLAY_HPP

#include <cstdint>
namespace device_layer
{
class IDisplay
{
   public:
    virtual ~IDisplay() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void update() = 0;
    virtual void drawPixel(std::uint32_t x, std::uint32_t y) = 0;
    virtual void clear() = 0;
};
}  // namespace device_layer
#endif  // I_DISPLAY_HPP
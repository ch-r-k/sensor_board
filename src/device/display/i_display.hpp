#ifndef I_DISPLAY_HPP
#define I_DISPLAY_HPP

#include <cstdint>
namespace device_layer
{
namespace interface
{
class IDisplay
{
   public:
    struct Color
    {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
    };

   public:
    virtual ~IDisplay() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void update() = 0;
    virtual void drawPixel(std::uint32_t x, std::uint32_t y, Color color) = 0;
    virtual void clear() = 0;
};
}  // namespace interface
}  // namespace device_layer
#endif  // I_DISPLAY_HPP
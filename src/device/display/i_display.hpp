#ifndef I_DISPLAY_HPP
#define I_DISPLAY_HPP

#include <cstdint>
class IDisplay
{
   public:
    virtual ~IDisplay() = default;
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void Update() = 0;
    virtual void DrawPixel(std::uint32_t x, std::uint32_t y) = 0;
    virtual void Clear() = 0;
};

#endif  // I_DISPLAY_HPP
#ifndef I_DISPLAY_HPP
#define I_DISPLAY_HPP

#include <cstdint>
class IDisplay
{
   public:
    virtual ~IDisplay() = default;
    virtual void Open() = 0;
    virtual void Close() = 0;
};

#endif  // I_DISPLAY_HPP
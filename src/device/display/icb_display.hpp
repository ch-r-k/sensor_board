#ifndef ICB_DISPLAY_HPP
#define ICB_DISPLAY_HPP

namespace device_layer
{

class IcbDisplay
{
   private:
   public:
    virtual ~IcbDisplay() = default;
    virtual void initDone() = 0;
    virtual void printDone() = 0;
};

}  // namespace device_layer
#endif  // ICB_DISPLAY_HPP
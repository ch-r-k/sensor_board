#ifndef ICB_DISPLAY_HPP
#define ICB_DISPLAY_HPP

class IcbDisplay
{
   private:
   public:
    virtual ~IcbDisplay() = default;
    virtual void initDone() = 0;
    virtual void printDone() = 0;
};

#endif  // ICB_DISPLAY_HPP
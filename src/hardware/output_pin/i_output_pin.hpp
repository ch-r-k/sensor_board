#ifndef I_OUTPUT_PIN_HPP
#define I_OUTPUT_PIN_HPP

class IOutputPin
{
   public:
    virtual void set() = 0;
    virtual void reset() = 0;
};

#endif  // I_OUTPUT_PIN_HPP
#ifndef I_USER_INDICATION_HPP
#define I_USER_INDICATION_HPP

namespace device_layer
{

class IUserIndication
{
   private:
   public:
    virtual void set() = 0;
    virtual void reset() = 0;
};

}  // namespace device_layer

#endif  // OUTPUT_PIN_HPP
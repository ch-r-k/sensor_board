#ifndef USER_INDICATION_HPP
#define USER_INDICATION_HPP

#include "i_user_indication.hpp"
#include "driver/output_pin/i_output_pin.hpp"

namespace device_layer
{

class UserIndication : public IUserIndication
{
   private:
    hardware_layer::interface::IOutputPin* outputPin = nullptr;

   public:
    void set() override;
    void reset() override;
    void setOutputPin(hardware_layer::interface::IOutputPin& init_output_pin);
};

}  // namespace device_layer

#endif  // USER_INDICATION_HPP
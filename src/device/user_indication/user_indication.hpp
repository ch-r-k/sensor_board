#ifndef USER_INDICATION_HPP
#define USER_INDICATION_HPP

#include "i_user_indication.hpp"
#include "i_output_pin.hpp"

class UserIndication : public IUserIndication
{
   private:
    hardware_layer::IOutputPin* outputPin = nullptr;

   public:
    void set() override;
    void reset() override;
    void setOutputPin(hardware_layer::IOutputPin& init_output_pin);
};

#endif  // USER_INDICATION_HPP
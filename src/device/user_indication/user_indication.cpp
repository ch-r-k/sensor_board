#include "user_indication.hpp"

namespace device_layer
{

void UserIndication::set() { outputPin->set(); }

void UserIndication::reset() { outputPin->reset(); }

void UserIndication::setOutputPin(
    hardware_layer::interface::IOutputPin& init_output_pin)
{
    this->outputPin = &init_output_pin;
}

}  // namespace device_layer
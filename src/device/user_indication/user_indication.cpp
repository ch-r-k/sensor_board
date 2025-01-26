#include "user_indication.hpp"

void UserIndication::set() { outputPin->set(); }

void UserIndication::reset() { outputPin->reset(); }

void UserIndication::setOutputPin(hardware_layer::IOutputPin& init_output_pin)
{
    this->outputPin = &init_output_pin;
}
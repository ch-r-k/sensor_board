#include "user_indication.hpp"

void UserIndication::set() { outputPin->set(); }

void UserIndication::reset() { outputPin->reset(); }

void UserIndication::setOutputPin(IOutputPin& initOutputPin)
{
    this->outputPin = &initOutputPin;
}
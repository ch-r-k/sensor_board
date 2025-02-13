#ifndef I_GUI_HPP_
#define I_GUI_HPP_

// std libs
#include <cstdint>
#include <string>

// 3d party libs
#include "qpcpp.hpp"

// app layer interfaces

// device layer interface

// signals

namespace app
{
class GuiLabel : public QP::QEvt
{
   public:
    std::uint8_t value;
    std::uint8_t identifier = 0;
};

class IGui
{
   public:
    virtual ~IGui() = default;
    virtual void setTempLabel(std::uint8_t value) = 0;
    virtual void setHumidityLabel(std::uint8_t value) = 0;
};

}  // namespace app

#endif  // I_GUI_HPP_

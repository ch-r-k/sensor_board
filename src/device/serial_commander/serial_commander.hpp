//============================================================================
//
//============================================================================

#ifndef SERIAL_COMMANDER_HPP_
#define SERIAL_COMMANDER_HPP_

#include <bits/c++config.h>
#include <array>
#include <cstdint>
#include <span>
#include "qpcpp.hpp"
#include "application_signals.hpp"

#include "i_serial_commander.hpp"
#include "icb_serial_commander.hpp"

#include "driver/i2c/i_i2c.hpp"
#include "driver/i2c/icb_i2c.hpp"
#include "driver/spi/i_spi.hpp"

namespace DEVICE
{
using Instructions = ISerialCommander::Instructions;
using Command = ISerialCommander::Command;

class CommandEvent : public QP::QEvt
{
   public:
    ISerialCommander::Command command;
};

class SerialCommander : public QP::QActive,
                        public ISerialCommander,
                        public IcbI2c
{
   private:
    QP::QTimeEvt m_timeEvt;
    IcbSerialCommander* icbSerialCommander = nullptr;
    II2c* iI2c = nullptr;
    ISpi* iSpi = nullptr;
    std::array<Command, 64> commands;
    std::size_t command_length = 0;
    std::size_t command_index = 0;

   private:
    QP::QStateHandler getStateFromInst(Instructions instruction);
    QP::QStateHandler nextState();
    void WriteDone() override;
    void ReadDone() override;
    void SetCommand(Command command) override;
    void StartCommands() override;

   public:
    SerialCommander();

    void setSerialInterface(II2c& i_i2c);
    void setSerialInterface(ISpi& i_i2c);
    void setIcbSerialCommander(IcbSerialCommander& icb_serial_commander);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(write);
    Q_STATE_DECL(read);
    Q_STATE_DECL(pause);
};

}  // namespace DEVICE

#endif  // SERIAL_COMMANDER_HPP_

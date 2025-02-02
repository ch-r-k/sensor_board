#ifndef SERIAL_COMMANDER_HPP_
#define SERIAL_COMMANDER_HPP_

#include <bits/c++config.h>
#include <array>
#include <cstdint>
#include <span>
#include "qpcpp.hpp"
#include "system_signals.hpp"

#include "i_serial_commander.hpp"
#include "icb_serial_commander.hpp"

#include "driver/i2c/i_i2c.hpp"
#include "driver/i2c/icb_i2c.hpp"
#include "driver/spi/i_spi.hpp"

using Instructions = ISerialCommander::Instructions;
using Command = ISerialCommander::Command;

class CommandEvent : public QP::QEvt
{
   public:
    ISerialCommander::Command command;
};

class SerialCommander : public QP::QActive,
                        public ISerialCommander,
                        public hardware_layer::IcbI2c
{
   private:
    QP::QTimeEvt m_timeEvt;
    IcbSerialCommander* icbSerialCommander = nullptr;
    hardware_layer::II2c* iI2c = nullptr;
    hardware_layer::ISpi* iSpi = nullptr;
    std::array<Command, 128> commands;
    std::size_t command_length = 0;
    std::size_t command_index = 0;

   private:
    QP::QStateHandler getStateFromInst(Instructions instruction);
    QP::QStateHandler nextState();
    void writeDone() override;
    void readDone() override;
    void setCommand(Command command) override;
    void startCommands() override;
    void setI2CAddress(std::uint8_t address) override;
    void setChipSelect(std::uint8_t pin) override;

   public:
    SerialCommander();

    void setSerialInterface(hardware_layer::II2c& i_i2c);
    void setSerialInterface(hardware_layer::ISpi& i_i2c);
    void setIcbSerialCommander(IcbSerialCommander& icb_serial_commander);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(write);
    Q_STATE_DECL(write_span);
    Q_STATE_DECL(read);
    Q_STATE_DECL(pause);
};

#endif  // SERIAL_COMMANDER_HPP_

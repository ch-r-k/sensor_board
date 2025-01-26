#include <bits/c++config.h>
#include <cassert>
#include <cstdint>
#include <span>

#include "qpcpp.hpp"
#include "system_signals.hpp"
#include "common.hpp"

#include "serial_commander.hpp"

#include "driver/i2c/i_i2c.hpp"
#include "stm32l4xx_hal_def.h"

// unnamed namespace for local definitions with internal linkage
namespace
{
// Q_DEFINE_THIS_FILE

}  // unnamed namespace

//............................................................................
SerialCommander::SerialCommander()
    : QP::QActive(&initial),
      m_timeEvt(this, system_layer::SERIAL_COMMANDER_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(SerialCommander, initial)
{
    (void)e;  // unused parameter

    QS_FUN_DICTIONARY(&initial);
    QS_FUN_DICTIONARY(&idle);
    QS_FUN_DICTIONARY(&write);
    QS_FUN_DICTIONARY(&write_span);
    QS_FUN_DICTIONARY(&read);
    QS_FUN_DICTIONARY(&pause);

    return tran(&idle);
}
//............................................................................
Q_STATE_DEF(SerialCommander, idle)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            command_length = 0;
            command_index = 0;

            commands.fill(Command{
                // todo: check if ok
                Instructions::PAUSE,
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                0U,
                {},
                0U,
            });

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SERIAL_COMMANDER_SET:
        {
            status = Q_RET_HANDLED;

            Command command = Q_EVT_CAST(CommandEvent)->command;

            commands[command_length].instruction = command.instruction;
            commands[command_length].data = command.data;
            commands[command_length].data_length = command.data_length;
            commands[command_length].pauseTime = command.pauseTime;
            commands[command_length].data_span = command.data_span;

            command_length++;
            break;
        }
        case system_layer::SERIAL_COMMANDER_START:
        {
            if (iI2c)
            {
                iI2c->open();
            }
            else if (iSpi)
            {
                iSpi->open();
            }
            else
            {
                assert(false && "no interface set");
            }

            status =
                tran(getStateFromInst(commands[command_index].instruction));
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
Q_STATE_DEF(SerialCommander, write)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            const std::span<const std::uint8_t> temp(
                commands[command_index].data.data(),
                commands[command_index].data_length);

            if (iI2c)
            {
                iI2c->startWrite(temp);
            }
            else if (iSpi)
            {
                iSpi->startWrite(temp);
            }
            else
            {
                assert(false && "no interface set");
            }

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SERIAL_COMMANDER_DONE:
        {
            status = tran(nextState());
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
Q_STATE_DEF(SerialCommander, write_span)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            const std::span<const std::uint8_t> temp(
                commands[command_index].data_span);

            if (iI2c)
            {
                iI2c->startWrite(temp);
            }
            else if (iSpi)
            {
                iSpi->startWrite(temp);
            }
            else
            {
                assert(false && "no interface set");
            }

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SERIAL_COMMANDER_DONE:
        {
            status = tran(nextState());
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
Q_STATE_DEF(SerialCommander, read)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            const std::span<std::uint8_t> temp(
                commands[command_index].data.data(),
                commands[command_index].data_length);

            if (iI2c)
            {
                iI2c->startRead(temp);
            }
            else if (iSpi)
            {
                iSpi->startRead(temp);
            }
            else
            {
                assert(false && "no interface set");
            }

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SERIAL_COMMANDER_DONE:
        {
            status = tran(nextState());
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
Q_STATE_DEF(SerialCommander, pause)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            std::uint16_t pauseTime = commands[command_index].pauseTime;

            m_timeEvt.armX(TICKS_PER_SEC / 1000U * pauseTime, 0);
            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::SERIAL_COMMANDER_TIMEOUT:
        {
            status = tran(nextState());
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}

//............................................................................
//............................................................................

QP::QStateHandler SerialCommander::nextState()
{
    IcbSerialCommander::ReturnValue returnValue;
    returnValue.data = commands[command_index].data;
    returnValue.data_length = commands[command_index].data_length;

    command_index++;

    if (command_index >= command_length)
    {
        if (iI2c)
        {
            iI2c->close();
        }
        else if (iSpi)
        {
            iSpi->close();
        }
        else
        {
            assert(false && "no interface set");
        }

        icbSerialCommander->done(returnValue);

        return &idle;
    }

    return getStateFromInst(commands[command_index].instruction);
}

QP::QStateHandler SerialCommander::getStateFromInst(Instructions instruction)
{
    assert(command_length > 0);

    switch (instruction)
    {
        case Instructions::WRITE:
        {
            return &write;
        }
        case Instructions::WRITE_SPAN:
        {
            return &write_span;
        }
        case Instructions::READ:
        {
            return &read;
        }
        case Instructions::PAUSE:
        {
            return &pause;
        }
    }

    assert(false);
}

void SerialCommander::setSerialInterface(hardware_layer::II2c& i_i2c)
{
    iSpi = nullptr;
    iI2c = &i_i2c;
}

void SerialCommander::setSerialInterface(hardware_layer::ISpi& i_spi)
{
    iI2c = nullptr;
    iSpi = &i_spi;
}

void SerialCommander::setIcbSerialCommander(
    IcbSerialCommander& icb_serial_commander)
{
    icbSerialCommander = &icb_serial_commander;
}

//............................................................................
//............................................................................
void SerialCommander::writeDone()
{
    static QP::QEvt const my_evt{system_layer::SERIAL_COMMANDER_DONE};
    this->POST(&my_evt, this);
}

void SerialCommander::readDone()
{
    static QP::QEvt const my_evt{system_layer::SERIAL_COMMANDER_DONE};
    this->POST(&my_evt, this);
}

void SerialCommander::setCommand(Command command)
{
    CommandEvent* commandEvent =
        Q_NEW(CommandEvent, system_layer::SERIAL_COMMANDER_SET);

    commandEvent->command.instruction = command.instruction;
    commandEvent->command.data = command.data;
    commandEvent->command.data_length = command.data_length;
    commandEvent->command.pauseTime = command.pauseTime;

    this->POST(commandEvent, this);
}

void SerialCommander::setI2CAddress(std::uint8_t address)
{
    iI2c->setAddress(address);
}
void SerialCommander::setChipSelect(std::uint8_t pin)
{
    UNUSED(pin);
    assert(false && "not implemented jet");
}

void SerialCommander::startCommands()
{
    static QP::QEvt const my_evt{system_layer::SERIAL_COMMANDER_START};
    this->POST(&my_evt, this);
}
//============================================================================
//
//============================================================================

#include <bits/c++config.h>
#include <cassert>
#include <cstdint>
#include <span>
#include "application_signals.hpp"
#include "driver/i2c/i_i2c.hpp"
#include "qpcpp.hpp"
#include "serial_commander.hpp"

// unnamed namespace for local definitions with internal linkage
namespace
{
// Q_DEFINE_THIS_FILE

}  // unnamed namespace

//............................................................................
SerialCommander::SerialCommander()
    : QP::QActive(&initial), m_timeEvt(this, APP::SERIAL_COMMANDER_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(SerialCommander, initial)
{
    (void)e;  // unused parameter

    // subscribe(AppSignals::BLINKY_DONE);
    // subscribe(AppSignals::SENSOR_DONE);

    // QS_FUN_DICTIONARY(&blinky);
    // QS_FUN_DICTIONARY(&sensor);
    // QS_FUN_DICTIONARY(&done);

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

            commands.fill(
                Command{Instructions::Pause,
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        0U,
                        0U});

            status = Q_RET_HANDLED;
            break;
        }
        case APP::AppSignals::SERIAL_COMMANDER_SET:
        {
            status = Q_RET_HANDLED;

             Command command = Q_EVT_CAST(CommandEvent)->command;

            commands[command_length].instruction = command.instruction;
            commands[command_length].data = command.data;
            commands[command_length].data_length = command.data_length;
            commands[command_length].pauseTime = command.pauseTime;

            command_length++;
            break;
        }
        case APP::AppSignals::SERIAL_COMMANDER_START:
        {
            if (iI2c)
            {
                iI2c->Open();
                iI2c->SetAddress(0x38);
            }
            else if (iSpi)
            {
                iSpi->Open();
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
                iI2c->StartWrite(temp);
            }
            else if (iSpi)
            {
                iSpi->StartWrite(temp);
            }
            else
            {
                assert(false && "no interface set");
            }

            status = Q_RET_HANDLED;
            break;
        }
        case APP::AppSignals::SERIAL_COMMANDER_DONE:
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
                iI2c->StartRead(temp);
            }
            else if (iSpi)
            {
                iSpi->StartRead(temp);
            }
            else
            {
                assert(false && "no interface set");
            }

            status = Q_RET_HANDLED;
            break;
        }
        case APP::AppSignals::SERIAL_COMMANDER_DONE:
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
            status = Q_RET_HANDLED;
            break;
        }
        case APP::AppSignals::SERIAL_COMMANDER_TIMEOUT:
        {
            status = Q_RET_HANDLED;
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
    command_index++;

    if (command_index >= command_length)
    {
        if (iI2c)
        {
            iI2c->Close();
        }
        else if (iSpi)
        {
            iSpi->Close();
        }
        else
        {
            assert(false && "no interface set");
        }
        
        // ToDo fix callback:
        //icbSerialCommander->ReadDone();
        //icbSerialCommander->WriteDone();

        return &idle;
    }

    return getStateFromInst(commands[command_index].instruction);
}

QP::QStateHandler SerialCommander::getStateFromInst(Instructions instruction)
{
    assert(command_length > 0);

    switch (instruction)
    {
        case Instructions::Write:
        {
            return &write;
        }
        case Instructions::Read:
        {
            return &read;
        }
        case Instructions::Pause:
        {
            return &pause;
        }
    }

    assert(false);
}

void SerialCommander::setSerialInterface(II2c& i_i2c)
{
    iSpi = nullptr;
    iI2c = &i_i2c;
}

void SerialCommander::setSerialInterface(ISpi& i_spi)
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
void SerialCommander::WriteDone()
{
    static QP::QEvt const myEvt{APP::AppSignals::SERIAL_COMMANDER_DONE};
    this->POST(&myEvt, this);
}

void SerialCommander::ReadDone()
{
    static QP::QEvt const myEvt{APP::AppSignals::SERIAL_COMMANDER_DONE};
    this->POST(&myEvt, this);
}

void SerialCommander::SetCommand(Command command)
{
    CommandEvent* commandEvent =
        Q_NEW(CommandEvent, APP::AppSignals::SERIAL_COMMANDER_SET);

    commandEvent->command.instruction = command.instruction;
    commandEvent->command.data = command.data;
    commandEvent->command.data_length = command.data_length;
    commandEvent->command.pauseTime = command.pauseTime;

    this->POST(commandEvent, this);
}

void SerialCommander::StartCommands()
{
    static QP::QEvt const myEvt{APP::AppSignals::SERIAL_COMMANDER_START};
    this->POST(&myEvt, this);
}
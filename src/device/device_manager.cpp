#include "device_manager.hpp"
#include <optional>
#include "sensors/aht10/aht10.hpp"

DeviceManager::DeviceManager(HardwareManager& hardware_manager)
{
    userIndication.setOutputPin(hardware_manager.getLedPin());
    // test.setSpiInterface(hardwareManager.getSensorSpi());
    // test.setI2cInterface(hardwareManager.getSensorI2c());

    // serial commander to sensor
    aht10.setSerialInterface(aoSensor);
    aoSensor.setIcbSerialCommander(aht10);
    aoSensor.setSerialInterface(hardware_manager.getSensorI2c());
    hardware_manager.getSensorI2c().setIcb(aoSensor);

    // serial commander to display
    ssd1306.setSerialCommanderInterface(aoDisplay);
    aoDisplay.setIcbSerialCommander(ssd1306);
    aoDisplay.setSerialInterface(hardware_manager.getDisplayI2c());
    hardware_manager.getDisplayI2c().setIcb(aoDisplay);

    // serial commander to hardware
}

DeviceManager::~DeviceManager() {}

UserIndication& DeviceManager::getUserIndication() { return userIndication; }
// Test& DeviceManager::getTest() {}
Aht10& DeviceManager::getAht10() { return aht10; };
Ssd1306& DeviceManager::getSsd1306() { return ssd1306; };

void DeviceManager::start()
{
    static QP::QEvt const* serial_sensor_queue_sto[10];
    aoSensor.start(5U,                              // QP prio. of the AO
                   serial_sensor_queue_sto,         // event queue storage
                   Q_DIM(serial_sensor_queue_sto),  // queue length [events]
                   nullptr, 0U,                     // no stack storage
                   nullptr);                        // no initialization param
    QS_OBJ_DICTIONARY(&aoSensor);

    static QP::QEvt const* serial_display_queue_sto[10];
    aoDisplay.start(6U,                               // QP prio. of the AO
                    serial_display_queue_sto,         // event queue storage
                    Q_DIM(serial_display_queue_sto),  // queue length [events]
                    nullptr, 0U,                      // no stack storage
                    nullptr);                         // no initialization param
    QS_OBJ_DICTIONARY(&aoDisplay);
}
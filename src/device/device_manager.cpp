#include "device_manager.hpp"
#include <optional>
#include "sensors/aht10/aht10.hpp"

DeviceManager::DeviceManager(HardwareManager& hardwareManager)
{
    userIndication.setOutputPin(hardwareManager.getLedPin());
    // test.setSpiInterface(hardwareManager.getSensorSpi());
    // test.setI2cInterface(hardwareManager.getSensorI2c());

    // serial commander to sensor
    aht10.setSerialInterface(aoSensor);
    aoSensor.setIcbSerialCommander(aht10);
    aoSensor.setSerialInterface(hardwareManager.getSensorI2c());
    hardwareManager.getSensorI2c().SetIcb(aoSensor);

    // serial commander to display
    ssd1306.setSerialCommanderInterface(aoDisplay);
    aoDisplay.setIcbSerialCommander(ssd1306);
    aoDisplay.setSerialInterface(hardwareManager.getDisplayI2c());
    hardwareManager.getDisplayI2c().SetIcb(aoDisplay);

    // serial commander to hardware
}

DeviceManager::~DeviceManager() {}

UserIndication& DeviceManager::getUserIndication() { return userIndication; }
// Test& DeviceManager::getTest() {}
Aht10& DeviceManager::getAht10() { return aht10; };
Ssd1306& DeviceManager::getSsd1306() { return ssd1306; };

void DeviceManager::start()
{
    static QP::QEvt const* serialSensorQueueSto[10];
    aoSensor.start(5U,                           // QP prio. of the AO
                   serialSensorQueueSto,         // event queue storage
                   Q_DIM(serialSensorQueueSto),  // queue length [events]
                   nullptr, 0U,                  // no stack storage
                   nullptr);                     // no initialization param
    QS_OBJ_DICTIONARY(&aoSensor);

    static QP::QEvt const* serialDisplayQueueSto[10];
    aoDisplay.start(6U,                            // QP prio. of the AO
                    serialDisplayQueueSto,         // event queue storage
                    Q_DIM(serialDisplayQueueSto),  // queue length [events]
                    nullptr, 0U,                   // no stack storage
                    nullptr);                      // no initialization param
    QS_OBJ_DICTIONARY(&aoDisplay);
}
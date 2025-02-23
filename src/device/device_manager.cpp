#include "device_manager.hpp"
#include <optional>
#include "sensors/aht10/aht10.hpp"

namespace manager
{

DeviceManager::DeviceManager(HardwareManager& hardware_manager)
{
    userIndication.setOutputPin(hardware_manager.getLedPin());

    // serial commander to sensor
    aht10.setSerialInterface(hardware_manager.getSensorSerial());
    hardware_manager.getSensorSerial().setIcbSerialCommander(aht10);

    // serial commander to display
    ssd1306.setSerialCommanderInterface(hardware_manager.getDisplaySerial());
    hardware_manager.getDisplaySerial().setIcbSerialCommander(ssd1306);
}

DeviceManager::~DeviceManager() {}

device_layer::UserIndication& DeviceManager::getUserIndication()
{
    return userIndication;
}
device_layer::Aht10& DeviceManager::getAht10() { return aht10; };
device_layer::Ssd1306& DeviceManager::getSsd1306() { return ssd1306; };

void DeviceManager::start() {}

}  // namespace manager
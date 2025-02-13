#include "application_manager.hpp"

ApplicationManager::ApplicationManager(DeviceManager& device_manager)
{
    // application manager
    // Sensor -> Gui
    aoSensor.setGuiInterface(aoGui);

    // device manager
    // blinky
    aoBlinky.setUserIndication(device_manager.getUserIndication());

    // sensor
    aoSensor.setSensorInterface(device_manager.getAht10());
    device_manager.getAht10().setIcbSensor(aoSensor);

    // gui
    aoGui.setDisplayInterface(device_manager.getSsd1306());
    device_manager.getSsd1306().setIcbDisplay(aoGui);
}

ApplicationManager::~ApplicationManager() {}

void ApplicationManager::start()
{
    // initialize publish-subscribe
    static QP::QSubscrList subscr_sto[system_layer::MAX_PUB_SIG];
    QP::QActive::psInit(subscr_sto, Q_DIM(subscr_sto));

    // instantiate and start AOs/threads...

    static QP::QEvt const* blinky_queue_sto[10];
    aoBlinky.start(1U,                       // QP prio. of the AO
                   blinky_queue_sto,         // event queue storage
                   Q_DIM(blinky_queue_sto),  // queue length [events]
                   nullptr, 0U,              // no stack storage
                   nullptr);                 // no initialization param
    QS_OBJ_DICTIONARY(&aoBlinky);

    static QP::QEvt const* sensor_queue_sto[10];
    aoSensor.start(2U,                       // QP prio. of the AO
                   sensor_queue_sto,         // event queue storage
                   Q_DIM(sensor_queue_sto),  // queue length [events]
                   nullptr, 0U,              // no stack storage
                   nullptr);                 // no initialization param
    QS_OBJ_DICTIONARY(&aoSensor);

    static QP::QEvt const* startup_queue_sto[10];
    aoStartup.start(3U,                        // QP prio. of the AO
                    startup_queue_sto,         // event queue storage
                    Q_DIM(startup_queue_sto),  // queue length [events]
                    nullptr, 0U,               // no stack storage
                    nullptr);                  // no initialization param
    QS_OBJ_DICTIONARY(&aoStartup);

    static QP::QEvt const* gui_queue_sto[10];
    aoGui.start(4U,                    // QP prio. of the AO
                gui_queue_sto,         // event queue storage
                Q_DIM(gui_queue_sto),  // queue length [events]
                nullptr, 0U,           // no stack storage
                nullptr);              // no initialization param
    QS_OBJ_DICTIONARY(&aoStartup);
}
#include "application_manager.hpp"

ApplicationManager::ApplicationManager(DeviceManager& deviceManager)
{
    aoBlinky.setUserIndication(deviceManager.getUserIndication());

    aoSensor.setSensorInterface(deviceManager.getAht10());
    deviceManager.getAht10().setIcbSensor(aoSensor);

    aoGui.setDisplayInterface(deviceManager.getSsd1306());
    deviceManager.getSsd1306().setIcbDisplay(aoGui);
}

ApplicationManager::~ApplicationManager() {}

void ApplicationManager::start()
{
    // initialize publish-subscribe
    static QP::QSubscrList subscrSto[APP::MAX_PUB_SIG];
    QP::QActive::psInit(subscrSto, Q_DIM(subscrSto));

    // instantiate and start AOs/threads...

    static QP::QEvt const* blinkyQueueSto[10];
    aoBlinky.start(1U,                     // QP prio. of the AO
                   blinkyQueueSto,         // event queue storage
                   Q_DIM(blinkyQueueSto),  // queue length [events]
                   nullptr, 0U,            // no stack storage
                   nullptr);               // no initialization param
    QS_OBJ_DICTIONARY(&aoBlinky);

    static QP::QEvt const* sensorQueueSto[10];
    aoSensor.start(2U,                     // QP prio. of the AO
                   sensorQueueSto,         // event queue storage
                   Q_DIM(sensorQueueSto),  // queue length [events]
                   nullptr, 0U,            // no stack storage
                   nullptr);               // no initialization param
    QS_OBJ_DICTIONARY(&aoSensor);

    static QP::QEvt const* startupQueueSto[10];
    aoStartup.start(3U,                      // QP prio. of the AO
                    startupQueueSto,         // event queue storage
                    Q_DIM(startupQueueSto),  // queue length [events]
                    nullptr, 0U,             // no stack storage
                    nullptr);                // no initialization param
    QS_OBJ_DICTIONARY(&aoStartup);

    static QP::QEvt const* guiQueueSto[10];
    aoGui.start(4U,                  // QP prio. of the AO
                guiQueueSto,         // event queue storage
                Q_DIM(guiQueueSto),  // queue length [events]
                nullptr, 0U,         // no stack storage
                nullptr);            // no initialization param
    QS_OBJ_DICTIONARY(&aoStartup);
}
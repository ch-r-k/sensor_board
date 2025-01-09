#include "application_manager.hpp"
#include "sensor/sensor.hpp"

ApplicationManager::ApplicationManager(DeviceManager& deviceManager)
{
    aoBlinky.setUserIndication(deviceManager.getUserIndication());

    aoSensor.setSensorInterface(deviceManager.getAht10());
    deviceManager.getAht10().setIcbSensor(aoSensor);
}

ApplicationManager::~ApplicationManager() {}

void ApplicationManager::start()
{
    // initialize event pools
    static QF_MPOOL_EL(APP::SensorEvent) smlPoolSto[20];
    QP::QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

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
}
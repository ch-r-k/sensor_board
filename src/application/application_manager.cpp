#include "application_manager.hpp"

ApplicationManager::ApplicationManager(DeviceManager& deviceManager)
{
    aoBlinky.setUserIndication(deviceManager.getUserIndication());
}

ApplicationManager::~ApplicationManager() {}

void ApplicationManager::start()
{
    // initialize event pools
    static QF_MPOOL_EL(QP::QEvt) smlPoolSto[20];
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
}
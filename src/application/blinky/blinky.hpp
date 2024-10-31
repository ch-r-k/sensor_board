//============================================================================
// Blinky example
//============================================================================

#ifndef BLINKY_HPP_
#define BLINKY_HPP_

#include "i_user_indication.hpp"

namespace APP
{
enum AppSignals : QP::QSignal
{
    DUMMY_SIG = QP::Q_USER_SIG,
    MAX_PUB_SIG,  // the last published signal

    TIMEOUT_SIG,
    MAX_SIG  // the last signal
};

class Blinky : public QP::QActive
{
   private:
    QP::QTimeEvt m_timeEvt;
    IUserIndication* userIndication = nullptr;

   public:
    Blinky();
    void setUserIndication(IUserIndication& initUserIndication);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(off);
    Q_STATE_DECL(on);
};

}  // namespace APP

#endif  // BLINKY_HPP_

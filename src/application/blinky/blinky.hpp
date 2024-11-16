//============================================================================
// Blinky example
//============================================================================

#ifndef BLINKY_HPP_
#define BLINKY_HPP_

#include "qpcpp.hpp"
#include "i_user_indication.hpp"
#include "application_signals.hpp"
namespace APP
{
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
    Q_STATE_DECL(idle);
    Q_STATE_DECL(off);
    Q_STATE_DECL(on);
};

}  // namespace APP

#endif  // BLINKY_HPP_

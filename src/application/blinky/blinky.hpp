#ifndef BLINKY_HPP_
#define BLINKY_HPP_

#include "qpcpp.hpp"
#include "i_user_indication.hpp"
#include "system_signals.hpp"
namespace app
{
class Blinky : public QP::QActive
{
   private:
    QP::QTimeEvt m_timeEvt;
    device_layer::IUserIndication* userIndication = nullptr;

   public:
    Blinky();
    void setUserIndication(device_layer::IUserIndication& init_user_indication);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(off);
    Q_STATE_DECL(on);
};

}  // namespace app

#endif  // BLINKY_HPP_

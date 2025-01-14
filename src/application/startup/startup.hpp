//============================================================================
// Blinky example
//============================================================================

#ifndef STARTUP_HPP_
#define STARTUP_HPP_

#include "qpcpp.hpp"
#include "application_signals.hpp"

namespace APP
{
class Startup : public QP::QActive
{
   private:
    QP::QTimeEvt m_timeEvt;

   public:
    Startup();

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(blinky);
    Q_STATE_DECL(sensor);
    Q_STATE_DECL(done);
};

}  // namespace APP

#endif  // STARTUP_HPP_

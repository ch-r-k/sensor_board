//============================================================================
// Blinky example
//============================================================================

#ifndef STARTUP_HPP_
#define STARTUP_HPP_

#include "qpcpp.hpp"
#include "application_signals.hpp"

namespace app
{
class Startup : public QP::QActive
{
   private:
   public:
    Startup();

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(blinky);
    Q_STATE_DECL(sensor);
    Q_STATE_DECL(display);
    Q_STATE_DECL(done);
};

}  // namespace app

#endif  // STARTUP_HPP_

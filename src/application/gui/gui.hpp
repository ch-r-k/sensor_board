#ifndef GUI_HPP_
#define GUI_HPP_

#include "qpcpp.hpp"
#include "device/display/i_display.hpp"
#include "device/display/icb_display.hpp"
#include "system_signals.hpp"

namespace app
{
class Gui : public QP::QActive, public IcbDisplay
{
   private:
    IDisplay* iDisplay = nullptr;

   public:
    Gui();
    void setDisplayInterface(IDisplay& i_display);

   private:
    void initDone() override;
    void printDone() override;

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(initialize);
    Q_STATE_DECL(update);
};

}  // namespace app

#endif  // GUI_HPP_

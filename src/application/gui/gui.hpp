#ifndef GUI_HPP_
#define GUI_HPP_

#include <cstdint>
#include "qpcpp.hpp"
#include "device/display/i_display.hpp"
#include "device/display/icb_display.hpp"
#include "system_signals.hpp"

#include "lvgl.h"

namespace app
{
class Gui : public QP::QActive, public device_layer::IcbDisplay
{
   private:
    QP::QTimeEvt m_timeEvt;
    device_layer::IDisplay *iDisplay = nullptr;

    lv_disp_draw_buf_t disp_buf;
    lv_color_t buf_1[128 * 64];

   public:
    Gui();
    void setDisplayInterface(device_layer::IDisplay &i_display);
    std::uint8_t x = 0;
    std::uint8_t y = 0;

   private:
    void initDone() override;
    void printDone() override;
    static void staticFlushCallback(lv_disp_drv_t *disp_drv,
                                    const lv_area_t *area, lv_color_t *color_p);

   protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(idle);
    Q_STATE_DECL(initialize);
    Q_STATE_DECL(update);
};

}  // namespace app

#endif  // GUI_HPP_

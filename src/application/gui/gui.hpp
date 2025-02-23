#ifndef GUI_HPP_
#define GUI_HPP_

// std libs
#include <cstdint>
#include <string>

// 3d party libs
#include "lvgl.h"
#include "qpcpp.hpp"

// app layer interfaces
#include "i_gui.hpp"

// device layer interface
#include "device/display/i_display.hpp"
#include "device/display/icb_display.hpp"

// signals
#include "system_signals.hpp"

namespace app
{
class Gui : public QP::QActive,
            public device_layer::interface::IcbDisplay,
            public IGui
{
   private:
    QP::QTimeEvt m_timeEvt;
    device_layer::interface::IDisplay *iDisplay = nullptr;

    lv_disp_draw_buf_t disp_buf;
    lv_color_t buf_1[128 * 64];

    lv_obj_t *labelTitle;
    lv_obj_t *labelTemperature;
    lv_obj_t *labelHumidity;

   public:
    Gui();
    void setDisplayInterface(device_layer::interface::IDisplay &i_display);
    void setTempLabel(std::uint8_t value) override;
    void setHumidityLabel(std::uint8_t value) override;
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

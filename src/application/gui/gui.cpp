#include "gui.hpp"
#include <bits/c++config.h>
#include <cstdint>
#include "system_signals.hpp"
#include "common.hpp"
#include "display/i_display.hpp"

namespace app
{
//............................................................................
Gui::Gui()
    : QP::QActive(&initial), m_timeEvt(this, system_layer::GUI_TIMEOUT, 0U)
{
    // empty
}

// HSM definition ------------------------------------------------------------
Q_STATE_DEF(Gui, initial)
{
    (void)e;  // unused parameter
    subscribe(system_layer::GUI_START);

    QS_FUN_DICTIONARY(&idle);
    QS_FUN_DICTIONARY(&initial);
    QS_FUN_DICTIONARY(&update);

    return tran(&idle);
}
//............................................................................
Q_STATE_DEF(Gui, idle)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::GUI_START:
        {
            static QP::QEvt const my_evt{system_layer::GUI_DONE};
            QP::QActive::PUBLISH(&my_evt, this);

            status = tran(&initialize);
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
Q_STATE_DEF(Gui, initialize)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            iDisplay->open();

            lv_init();
            lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, sizeof(buf_1));

            static lv_disp_drv_t disp_drv;
            /*Set the resolution of the display*/
            lv_disp_drv_init(&disp_drv);
            disp_drv.draw_buf = &disp_buf;
            disp_drv.user_data = this;
            disp_drv.hor_res = 128;
            disp_drv.ver_res = 64;
            disp_drv.direct_mode = 0;
            disp_drv.antialiasing = 1;
            disp_drv.full_refresh = 0;
            disp_drv.dpi = 167;

            /*Used to copy the buffer's content to the display*/
            disp_drv.flush_cb = staticFlushCallback;

            lv_disp_t *disp;
            disp = lv_disp_drv_register(&disp_drv);

            //
            /*{
                lv_obj_t *label = lv_label_create(lv_scr_act());
                lv_label_set_text(label, "Hello world");
                lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            } */

            {
                /*Create a menu object*/
                lv_obj_t *menu = lv_menu_create(lv_scr_act());
                lv_obj_set_size(menu, 128, 64);
                lv_obj_center(menu);

                lv_obj_t *cont;
                lv_obj_t *label;

                /*Create a sub page*/
                lv_obj_t *sub_page = lv_menu_page_create(menu, NULL);

                cont = lv_menu_cont_create(sub_page);
                label = lv_label_create(cont);
                lv_label_set_text(label, "Hello");

                /*Create a main page*/
                lv_obj_t *main_page = lv_menu_page_create(menu, NULL);

                cont = lv_menu_cont_create(main_page);
                label = lv_label_create(cont);
                lv_label_set_text(label, "Item 1");

                cont = lv_menu_cont_create(main_page);
                label = lv_label_create(cont);
                lv_label_set_text(label, "Item 2");

                lv_menu_set_load_page_event(menu, cont, sub_page);

                lv_menu_set_page(menu, main_page);
            }

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::GUI_INIT_DONE:
        {
            status = tran(&update);
            x = 0;
            y = 0;
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
Q_STATE_DEF(Gui, update)
{
    QP::QState status;
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            lv_timer_handler();
            m_timeEvt.armX(TICKS_PER_SEC / 50U, 0);

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::GUI_TIMEOUT:
        {
            lv_timer_handler();
            m_timeEvt.armX(TICKS_PER_SEC / 50U, 0);

            status = Q_RET_HANDLED;
            break;
        }
        case system_layer::GUI_UPDATE_DONE:
        {
            status = Q_RET_HANDLED;
            break;
        }
        default:
        {
            status = super(&top);
            break;
        }
    }
    return status;
}
//............................................................................
void Gui::setDisplayInterface(device_layer::IDisplay &i_display)
{
    iDisplay = &i_display;
}

void Gui::initDone()
{
    static QP::QEvt const my_evt{system_layer::GUI_INIT_DONE};
    this->POST(&my_evt, this);
}

void Gui::printDone()
{
    static QP::QEvt const my_evt{system_layer::GUI_UPDATE_DONE};
    this->POST(&my_evt, this);
}

// Static function that calls the member function
void Gui::staticFlushCallback(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                              lv_color_t *color_p)
{
    Gui *self = static_cast<Gui *>(disp_drv->user_data);
    if (!self) return;

    // Iterate over the area provided by LVGL
    for (int y = area->y1; y <= area->y2; y++)
    {
        for (int x = area->x1; x <= area->x2; x++)
        {
            // Extract pixel color (1-bit depth: 0 = off, 1 = on)
            device_layer::IDisplay::Color pixel_color;
            pixel_color.red = lv_color_to1(*color_p);
            pixel_color.green = lv_color_to1(*color_p);
            pixel_color.blue = lv_color_to1(*color_p);

            // Set the pixel in the display buffer
            self->iDisplay->drawPixel(127 - x, y, pixel_color);

            // Move to the next pixel in the buffer
            color_p++;
        }
    }

    // Push updates to the display
    self->iDisplay->update();

    // Notify LVGL that flushing is done
    lv_disp_flush_ready(disp_drv);
}

}  // namespace app

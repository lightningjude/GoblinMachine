#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "liblvgl/llemu.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "liblvgl/lvgl.h"
#include <cstdint>

pros::Color c=pros::Color::black;
int cp=NULL;
int side=NULL;
int skills=0;


static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_buttonmatrix_get_selected_button(obj);
        const char * txt = lv_buttonmatrix_get_button_text(obj, id);
        LV_UNUSED(txt);
        LV_LOG_USER("%s was pressed\n", txt);
    }
}
static void color_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_buttonmatrix_get_selected_button(obj);
        if (id==0) {
            //red pressed
            c=pros::Color::red;
            cp=0;
        
    }
        else if (id==1) {
            //blue pressed
            c=pros::Color::blue;
            cp=1;
        }
          
}
}
static void side_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_buttonmatrix_get_selected_button(obj);
        if (id==0) {
            //left pressed
            side=0;
        
    }
        else if (id==1) {
            //right pressed
            side=1;
        }
    
}
}
static void mode_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_buttonmatrix_get_selected_button(obj);
        if (id==0) {
            //match pressed
            skills=0;
        
    }
        else if (id==1) {
            //skills pressed
            skills=1;
        }
          
}
}
static const char * btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                                  "6", "7", "8", "9", "0", "\n",
                                  "Action1", "Action2", ""
                                 };
static const char * color_btnm_map[] = {"Red", "Blue",""};
static const char * side_btnm_map[] = {"Left","Right", ""};
static const char * mode_btnm_map[] = {"Match","Skills",""};

void lv_example_buttonmatrix_1(void)
{
    lv_obj_t * btnm1 = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(btnm1, btnm_map);
    lv_buttonmatrix_set_button_width(btnm1, 10, 2);        /*Make "Action1" twice as wide as "Action2"*/
    lv_buttonmatrix_set_button_ctrl_all(btnm1, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    
   
    lv_obj_align(btnm1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btnm1, event_handler, LV_EVENT_PRESSED, NULL);
}
void lv_color_buttonmatrix(void)
{
    lv_obj_t * color_btnm = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(color_btnm, color_btnm_map);
    lv_buttonmatrix_set_button_ctrl_all(color_btnm, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    
   
    lv_obj_align(color_btnm, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(color_btnm, color_event_handler, LV_EVENT_PRESSED, NULL);
}
void lv_side_buttonmatrix(void)
{
    lv_obj_t * side_btnm = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(side_btnm, side_btnm_map);
    lv_buttonmatrix_set_button_ctrl_all(side_btnm, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    
   
    lv_obj_align(side_btnm, LV_ALIGN_CENTER, 0, 120);
    lv_obj_add_event_cb(side_btnm, side_event_handler, LV_EVENT_PRESSED, NULL);
}
void lv_mode_buttonmatrix(void)
{
    lv_obj_t * mode_btnm = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(mode_btnm, mode_btnm_map);
    lv_buttonmatrix_set_button_ctrl_all(mode_btnm, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    
   
    lv_obj_align(mode_btnm, LV_ALIGN_CENTER, 0, -60);
    lv_obj_add_event_cb(mode_btnm, mode_event_handler, LV_EVENT_PRESSED, NULL);
}
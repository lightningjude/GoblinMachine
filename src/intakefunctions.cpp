#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/misc/lv_types.h"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"

//intake motors

pros::Motor intakelow(-7, pros::MotorGearset::blue);
pros::Motor intakehigh(-8, pros::MotorGearset::blue);
//pneumatics
pros::adi::Pneumatics scorer=pros::adi::Pneumatics('b',true);
pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
void intakein() {
    lv_obj_t * intakel=lv_label_create(lv_screen_active());
    lv_label_set_text(intakel, "Intaking");
    lv_obj_align(intakel, LV_ALIGN_CENTER, 0, 5);
    intakelow.move(127);
    intakehigh.move(0);
    scorer.extend();
}
void outup() {
    intakelow.move(127);
    intakehigh.move(127);
    scorer.extend();
}
void outdown() {
    intakelow.move(-127);
    intakehigh.move(-127);
    scorer.extend();
}
void outmiddle() {
    intakelow.move(127);
    intakehigh.move(-127);
    scorer.retract();
}
void intakestop() {
    intakelow.move(0);
    intakehigh.move(0);
}
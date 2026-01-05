#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"


void intakein() {
    intakelow.move_velocity(100);
    intakehigh.move_velocity(100);
    scorer.extend();
}
void outup() {
    intakelow.move_velocity(100);
    intakehigh.move_velocity(100);
    scorer.extend();
}
void outdown() {
    intakelow.move_velocity(-100);
    intakehigh.move_velocity(-100);
}
void outmiddle() {
    intakelow.move_velocity(100);
    intakehigh.move_velocity(-100);
    scorer.retract();
}
void intakestop() {
    intakelow.move_velocity(0);
    intakehigh.move_velocity(0);
}
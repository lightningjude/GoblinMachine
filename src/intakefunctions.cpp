#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"

//intake motors

pros::Motor intakelow(7, pros::MotorGearset::green);
pros::Motor intakehigh(8, pros::MotorGearset::green);
//pneumatics
pros::adi::Pneumatics scorer=pros::adi::Pneumatics('b',true);
pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
void intakein() {
    intakelow.move_velocity(100);
    intakehigh.move_velocity(-100);
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
    scorer.extend();
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
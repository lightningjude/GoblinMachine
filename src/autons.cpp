#include "lemlib/asset.hpp"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"


void autonlb(lemlib::Chassis chassis) {
    //left blue
    chassis.setPose(0,0,0);
    chassis.moveToPose(0, 10, 0, 1000);
}
void autonrb(lemlib::Chassis chassis) {
    //right blue
    chassis.setPose(0,0,0);
    chassis.moveToPose(0, 10, 0, 1000);
}
void autonlr(lemlib::Chassis chassis) {
    //left red
    chassis.setPose(0,0,0);
    chassis.moveToPose(0, 10, 0, 1000);
}
void autonrr(lemlib::Chassis chassis) {
    //right red
    chassis.setPose(0,0,0);
    chassis.moveToPose(0, 10, 0, 1000);
}

ASSET(skillsp1_txt);
ASSET(skillsp2_txt);
ASSET(skillsp3_txt);
ASSET(skillsp4_txt);
void autonskills(lemlib::Chassis chassis) {
    //skills
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
    chassis.setPose(0,0,0);
    matchload.extend();
    chassis.follow(skillsp1_txt, 10, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    chassis.follow(skillsp2_txt, 10, 10000);
    outup();
    pros::delay(5000);
    intakestop();
    chassis.follow(skillsp3_txt, 10, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    chassis.follow(skillsp4_txt, 10, 10000);
    outup();
    pros::delay(5000);
    intakestop();
}
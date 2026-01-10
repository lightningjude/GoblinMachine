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

ASSET(example_txt);
void autonskills(lemlib::Chassis chassis) {
    //skills
    chassis.setPose(0,0,0);
    chassis.follow(example_txt, 10, 2000);
}
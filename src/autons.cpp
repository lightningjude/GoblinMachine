#include "lemlib/asset.hpp"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <ctime>


void autonlb(lemlib::Chassis chassis) {
    //left blue
    chassis.setPose(0,0,0);
    
}
void autonrb(lemlib::Chassis chassis) {
    //right blue
    chassis.setPose(0,0,0);
    
}
void autonlr(lemlib::Chassis chassis) {
    //left red
    chassis.setPose(0,0,0);
    
}
void autonrr(lemlib::Chassis chassis) {
    //right red
    chassis.setPose(0,0,0);
    
}
using FunctionPointer = void (*)();
ASSET(skillsp1_txt);
ASSET(skillsp2_txt);
ASSET(skillsp3_txt);
ASSET(skillsp4_txt);
ASSET(skillsp5_txt);
void matchextend() {
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
    matchload.extend();
}
FunctionPointer func=&matchextend;
int timed=2000;
void timefunc() {
    
    pros::delay(timed);
    func();
}
void autonskills(lemlib::Chassis chassis) {
    //skills
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
    //setup
    chassis.setPose(0,0,0);
    //prep for load
    matchload.extend();
    //drive to match loader
    chassis.follow(skillsp1_txt, 10, 10000);
    //intake in blobks for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //stop, then drive to opposite side of big goal
    chassis.follow(skillsp2_txt, 10, 10000);
    //score blocks in big goal for 5 sec
    outup();
    pros::delay(5000);
    intakestop();
    //stop, then back up to opposite match loader
    chassis.follow(skillsp3_txt, 10, 10000);
    //intake in blocks for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //stop, then drive to big goal again
    chassis.follow(skillsp4_txt, 10, 10000);
    outup();
    pros::delay(5000);
    intakestop();
    //stop, retract match load  to prep for parking clear
    matchload.retract();
    pros::Task time_task(timefunc);
    chassis.follow(skillsp5_txt, 10, 10000);
}
#include "lemlib/asset.hpp"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <cstdint>
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
ASSET(skillsp6_txt);
ASSET(skillsp7_txt);
ASSET(skillsp8_txt);
ASSET(skillsp9_txt);
void matchextend() {
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
    matchload.extend();
}
void matchretract() {
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
    matchload.retract();
}
void timefuncext(void* delay) {
    int timed = *((int*)delay);
    pros::delay(timed);
    
    matchextend();
}
void timefuncrt(void* delay) {
    int timed = *((int*)delay);
    pros::delay(timed);
    matchretract();
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
    //drive across parking, matchload extends on a 2 sec delay
    int timed=2000;
    pros::Task time_task (timefuncext,(void*)timed,"Extend1");
    chassis.follow(skillsp5_txt, 10, 10000);
    //intake from match load for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //drive across to opposite long goal side
    chassis.follow(skillsp6_txt, 10, 10000);
    outup();
    pros::delay(5000);
    intakestop();
    //back up to opposite mobile goal
    chassis.follow(skillsp7_txt, 10, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    //go back to long goal
    chassis.follow(skillsp8_txt, 10,10000);
    outup();
    pros::delay(5000);
    intakestop();
    //drive through parking, then around and back in
    chassis.follow(skillsp9_txt,10,10000);
}
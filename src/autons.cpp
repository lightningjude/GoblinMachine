#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <cstdint>
#include <ctime>

void timefuncdscore(void* delay) {
    int timed=*((int*)delay);
    pros::delay(timed);
    outdown();
}


ASSET(leftsideautons_txt);
ASSET(rightsideautons1_txt);
ASSET(rightsideautons2_txt);
void autonlb(lemlib::Chassis* robot) {
    //left blue
    robot->setPose(0,0,0);
    intakein();
    int bruh = 2000;
    pros::Task wait_task (timefuncdscore, (void*)bruh,"dude");
    robot->follow(leftsideautons_txt, 10, 10000);
    intakestop();
    float prev = robot->getPose().theta;
    robot->setPose(0,0,prev);
    robot->moveToPose(0, -4, 0, 3000);
}
void autonrb(lemlib::Chassis* robot) {
    //right blue
    robot->setPose(0,0,0);
    robot->follow(rightsideautons1_txt, 10, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    robot->follow(rightsideautons2_txt, 10, 10000);
    outup();
    pros::delay(5000);
    intakestop();    
}
void autonlr(lemlib::Chassis* robot) {
    //left red
    robot->setPose(0,0,0);
    intakein();
    int bruh = 4000;
    pros::Task wait_task (timefuncdscore, (void*)bruh,"dude");
    robot->follow(leftsideautons_txt, 10, 10000);
    intakestop();
    float prev = robot->getPose().theta;
    robot->setPose(0,0,prev);
    robot->moveToPose(0, -4, 0, 3000);
    
}
void autonrr(lemlib::Chassis* robot) {
    //right red
    robot->setPose(0,0,0);
    robot->follow(rightsideautons1_txt, 10, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    robot->follow(rightsideautons2_txt, 10, 10000,false);
    outup();
    pros::delay(5000);
    intakestop();  
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
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',false);
    matchload.extend();
}
void matchretract() {
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',false);
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
void autonskills(lemlib::Chassis* robot) {
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',false);
    bool revpaths = true;
    //setup
    robot->setPose(0,0,0);
    //prep for load
    matchload.extend();
    //drive to match loader
    robot->follow(skillsp1_txt, 10, 10000);
    //intake in blobks for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //stop, then drive to opposite side of big goal
    robot->follow(skillsp2_txt, 10, 10000,!revpaths);
    //score blocks in big goal for 5 sec
    outup();
    pros::delay(5000);
    intakestop();
    //stop, then back up to opposite match loader
    robot->follow(skillsp3_txt, 10, 10000);
    //intake in blocks for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //stop, then drive to big goal again
    robot->follow(skillsp4_txt, 10, 10000,!revpaths);
    outup();
    pros::delay(5000);
    intakestop();
    //stop, retract match load  to prep for parking clear
    matchload.retract();
    //drive across parking, matchload extends on a 2 sec delay
    int timed=2000;
    pros::Task time_task (timefuncext,(void*)timed,"Extend1");
    robot->follow(skillsp5_txt, 10, 10000);
    //intake from match load for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //drive across to opposite long goal side
    robot->follow(skillsp6_txt, 10, 10000,!revpaths);
    outup();
    pros::delay(5000);
    intakestop();
    //drive forward to opposite mobile goal
    robot->follow(skillsp7_txt, 10, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    //go back to long goal
    robot->follow(skillsp8_txt, 10,10000,!revpaths);
    outup();
    pros::delay(5000);
    intakestop();
    //drive through parking, then around and back in
    robot->follow(skillsp9_txt,10,10000);
}
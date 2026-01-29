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

void autonskillshand(lemlib::Chassis* robot) {


    //documentation for lemlib: 

    //all turns(including swing): https://lemlib.readthedocs.io/en/stable/tutorials/5_angular_motion.html

    //all movements that include fwd/bwd: https://lemlib.readthedocs.io/en/stable/tutorials/6_lateral_motion.html


    //THIS ONE IS IMPORTANT, READ CAREFULLY
    //motion chaining: This allows you to do turns around an object 
    // using multiple movetopose commands without stopping at each one
    //https://lemlib.readthedocs.io/en/stable/tutorials/8_motion_chaining.html


    //to do commands replace . with ->
    //ex robot->setPose(0,0,0);
    // or robot->moveToPose(10,10,90,10000);

    /*
    //there are two way to set the robots pose
    robot->setPose(0,0,0);
    //or
    lemlib::Pose base(0,0,0);
    robot->setPose(base);
    */


    /*
    //THERE IS ONLY ONE WAY FOR MOVETOPOSE OR MOVETOPOINT
    robot->moveToPose(0, 1, 0, 1000);
    //timeout is in milliseconds

    //move to pose DOES NOT accept lemlib::Pose objects, only the values
    //so this DOESN'T work
    lemlib::Pose goal(0,5,0);
    robot->moveToPose(goal,10000);
    //DO NOT DO THIS, it will throw an error
    */

    //movetopose accepts several parameters, which are passed like so:

    //1. max and min speed(out of 127), useful for motion chaining:
    //for this to work, you need to use params, example:
    //robot->moveToPose(5, 5, 45, 5000,{.maxSpeed=70,.minSpeed=50});

    //2. 
    


    //All movements happen in a separate task, so you can run other commands here while that happens

    //ex:
    /*
    robot->moveToPose(0,20,0,5000);
    //this runs asynchronously in another task, meaning you can do things like this

    //make intake run 1 second into drive
    pros::delay(1000);
    intakein();

    //since the moves are in another task, the previous commands run DURING the drive, 
    // not after it, if you want to wait till its done you can do this:
    while (robot->isInMotion()) {
        pros::delay(20);
    }
    //then place code here

    */
    
    pros::adi::Pneumatics scorerbruh=pros::adi::Pneumatics('b',true);
    pros::adi::Pneumatics matchloader=pros::adi::Pneumatics('a',false);
    //start outside end parking
    robot->setPose(-47.085,0,0);
    robot->moveToPose(-47.085,5,0,2000,{.minSpeed=127});
    matchloader.extend();
    robot->moveToPose(-40,44,0,5000,{.minSpeed=10});
    robot->moveToPose(-54.815,46.765,270,5000);
    while (robot->isInMotion()) {
        pros::delay(20);
    }
    intakein();
    pros::delay(5000);
    intakestop();
    robot->moveToPose(-50.815, 46.765, 270, 5000,{.forwards=false});
    matchloader.retract();
    robot->turnToHeading(45, 2000);
    robot->moveToPose(0, 60, 90, 5000,{.maxSpeed=80});
    robot->moveToPose(32,56,135,2000,{.minSpeed=80});
    robot->moveToPose(40,46.765,90,5000);
    robot->moveToPose(32.625,46.765,90,3000);
    while(robot->isInMotion()) {
        pros::delay(20);
    }
    outup();
    pros::delay(5000);
    intakestop();


}




void timefuncdscore(void* delay) {
    int timed=*((int*)delay);
    pros::delay(timed);
    outdown();
}


ASSET(leftsideautons_txt);
ASSET(rightsideautons1_txt);
ASSET(rightsideautons2_txt);
void autonlb(lemlib::Chassis* robot) {
    robot->setPose(-47.085,15.685,90);
    //left blue
    robot->follow(leftsideautons_txt, 3, 10000);
    intakein();
    int bruh = 4000;
    pros::delay(bruh); 
    outdown();
    while (robot->isInMotion()) {
        pros::delay(20);
    }
    intakestop();
    float prev = robot->getPose().theta;
    pros::delay(1000);
    robot->setPose(0,0,prev);
    robot->moveToPose(0, -4, prev, 3000,{.forwards=false});
}
void autonrb(lemlib::Chassis* robot) {
    robot->setPose(-49.085,-17.685,180);
    //right blue
    
    robot->follow(rightsideautons1_txt, 5, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    robot->follow(rightsideautons2_txt, 5, 10000);
    outup();
    pros::delay(5000);
    intakestop();    
}
void autonlr(lemlib::Chassis* robot) {
    robot->setPose(-47.085,15.685,90);
    //left red
    robot->follow(leftsideautons_txt, 5, 10000);
    intakein();
    int bruh = 6000;
    pros::delay(bruh);
    outdown(); 
    while (robot->isInMotion()) {
        pros::delay(20);
    }
    intakestop();
    float prev = robot->getPose().theta;
    robot->setPose(0,0,prev);
    robot->moveToPose(0, -4, 0, 3000);
    
}
void autonrr(lemlib::Chassis* robot) {
    robot->setPose(-49.085,-17.685,180);
    //right red
    robot->follow(rightsideautons1_txt, 5, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    robot->follow(rightsideautons2_txt, 5, 10000,false);
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


//HEYY, replace all tasks that involve delayed whatver with just that code in main task, as drive tasks run asyn anyway

void autonskills(lemlib::Chassis* robot) {
    robot->setPose(-47.085,0,0);
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',false);
    bool revpaths = true;
    //setup
    robot->setPose(-47.085,0,0);
    //prep for load
    matchload.extend();
    //drive to match loader
    robot->follow(skillsp1_txt, 5, 10000, true);
    //intake in blobks for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //stop, then drive to opposite side of big goal
    robot->follow(skillsp2_txt, 5, 10000,!revpaths);
    //score blocks in big goal for 5 sec
    outup();
    pros::delay(5000);
    intakestop();
    //stop, then back up to opposite match loader
    robot->follow(skillsp3_txt, 5, 10000);
    //intake in blocks for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //stop, then drive to big goal again
    robot->follow(skillsp4_txt, 5, 10000,!revpaths);
    outup();
    pros::delay(5000);
    intakestop();
    //stop, retract match load  to prep for parking clear
    matchload.retract();
    //drive across parking, matchload extends on a 2 sec delay
    int timed=2000;
    pros::Task time_task (timefuncext,(void*)timed,"Extend1");
    robot->follow(skillsp5_txt, 5, 10000);
    //intake from match load for 5 sec
    intakein();
    pros::delay(5000);
    intakestop();
    //drive across to opposite long goal side
    robot->follow(skillsp6_txt, 5, 10000,!revpaths);
    outup();
    pros::delay(5000);
    intakestop();
    //drive forward to opposite mobile goal
    robot->follow(skillsp7_txt, 5, 10000);
    intakein();
    pros::delay(5000);
    intakestop();
    //go back to long goal
    robot->follow(skillsp8_txt, 5,10000,!revpaths);
    outup();
    pros::delay(5000);
    intakestop();
    //drive through parking, then around and back in
    robot->follow(skillsp9_txt,5,10000);
}
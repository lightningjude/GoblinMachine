#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <ctime>
#include <string>
//cgeck

void autonskillshand(lemlib::Chassis* robot) {
    auto waittildone = [&](bool screen=true) {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    float counter =0;
    while (robot->isInMotion()) {

        pros::delay(20);
        if (screen) {
        master.clear();
        pros::delay(50);
        master.print(0, 0, std::to_string(counter).c_str());
        counter++;
        }
    }
};
    auto relmovelat= [&](float dist, int timeout,bool wait=false, lemlib::MoveToPoseParams bruh={}, bool async=true) {
    float oldt=robot->getPose().theta;
    robot->setPose(0,0,0);
    robot->moveToPose(0, dist, 0, timeout,bruh,async);
    float newt;
    if (wait) {
        waittildone();
        newt=robot->getPose().theta;
        if (newt>350) {
            newt=(newt-360)+oldt;
        }
        else {
            newt=newt+oldt;
        }
        robot->setPose(0,dist,newt);
    }
    else {
        pros::Task delay([&]{
            waittildone(false);
            newt=robot->getPose().theta;
            if (newt>350) {
                newt=(newt-360)+oldt;
            }
            else {
                newt=newt+oldt;
            }
            robot->setPose(0,dist,newt);
        });
    }
};
    auto relmovepose=[&](float x, float y, float theta, int timeout,bool wait=false, lemlib::MoveToPoseParams bruh ={}, bool async=true) {
    float oldt=robot->getPose().theta;
    robot->setPose(0,0,oldt);
    robot->moveToPose(x, y, theta, timeout,bruh,async);
    float newt;
    if (wait) {
        waittildone();
        newt=robot->getPose().theta;
        robot->setPose(x,y,newt);
    }
    else {
        pros::Task delay([&]{
            waittildone(false);
            newt=robot->getPose().theta;
            robot->setPose(x,y,newt);
        });
    }
};
auto chainmovepose=[&](float x, float y, float theta, int timeout,bool wait=false, lemlib::MoveToPoseParams bruh ={}, bool async=true) {
    robot->moveToPose(x, y, theta, timeout,bruh,async);
    float newt;
    if (wait) {
        waittildone();
        newt=robot->getPose().theta; 
        robot->setPose(x,y,newt);
    }
    else {
        pros::Task delay([&]{
            waittildone(false);
            newt=robot->getPose().theta;
            robot->setPose(x,y,newt);
        });
    }
};
auto relmovepoint=[&](float x, float y, int timeout,bool wait=false, lemlib::MoveToPointParams bruh ={}, bool async=true) {
    float oldt=robot->getPose().theta;
    robot->setPose(0,0,oldt);
    robot->moveToPoint(x, y, timeout,bruh,async);
    float newt;
    if (wait) {
        waittildone();
        newt=robot->getPose().theta;
        robot->setPose(x,y,newt);
    }
    else {
        pros::Task delay([&]{
            waittildone(false);
            newt=robot->getPose().theta;
            robot->setPose(x,y,newt);
        });
    }
};
auto relswingpoint = [&](float x, float y, lemlib::DriveSide lockside, int timeout, bool wait,lemlib::SwingToPointParams bruh={},bool async=true) {
    float oldt=robot->getPose().theta;
    robot->setPose(0,0,oldt);
    robot->swingToPoint(x, y, lockside, timeout,bruh);
    if (wait) {
        waittildone();
    }
    
};
auto relturnpoint=[&](float x, float y, int timeout,bool wait=false, lemlib::TurnToPointParams bruh ={}, bool async=true) {
    float oldt=robot->getPose().theta;
    robot->setPose(0,0,oldt);
    robot->turnToPoint(x, y, timeout,bruh,async);
    if (wait) {
        waittildone();
    }
};
auto posreset=[&]() {
    float oldt=robot->getPose().theta;
    robot->setPose(0,0,oldt);
};
    //documentation for lemlib: 
    //before any turn run this command:
    //posreset();


    //robot->TurnToHeading(angle in degrees, timeout in ms, optional parameters);

    
    //all turns(including swing): https://lemlib.readthedocs.io/en/stable/tutorials/5_angular_motion.html

    //piddrive documentation
    //It works like this:
    //first arg is distance, positive or negative, positive is intake faces direction of motion, negative is aligner faces direction of motion
    //second arg is timeout, in milliseconds
    //optional third arg is maxspeed, used for low-speed drives or when you need to go slow, it is out of 127

    //Plan A
    /*
    1. start on the side of parking
    2. drive straight down to even with mobile goal
    3. drive to mobile goal, intake, jiggle, back up
    4. Drive across to other side
    5. Get even with mobile goal/ long goal
    6. Backup into long goal, up score
    7. Extend match loader, drive to mobile goal, intake, jiggle, back up
    8. Back up to long goal, up score
    9. Back up, turn to face parking, drive through parking to other side, then even with other side mobile goal
    10. Repeat 3-9
    11. Clear other parking, then park
    12. Celebrate

    //Plan B(More complex, but higher scoring)-Max this is the instagram video
    1. Do steps 1-10 of plan A, but stop at step 8 on repeat
    2. On step 9, intake from the parking, 
    3. go to middle and low score
    4. Park in red parking
    5. Celebrate harder

    
    
    
    */    

    


  

   
    
    int done;
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    pros::adi::Pneumatics scorerbruh=pros::adi::Pneumatics('b',true);
    pros::adi::Pneumatics matchloader=pros::adi::Pneumatics('a',false);
    //start outside end parking, center axel centered on center nut of parking
    robot->setPose(0,0,0);
    pros::delay(20);
    done=0;
    //Drive to be even with match loader
    pros::Task moved ([&] {
        done=piddrive(51,2000);
    });
    while (done==0) {
        pros::delay(20);
    }
    
    
    //turn to face match loader w/ intake, extend match loader
    robot->turnToHeading(90, 1500,{.direction=AngularDirection::CW_CLOCKWISE});
    matchloader.extend();
    waittildone();
    done=0;
    //drive forward to match loader while intaking
    pros::Task move2 ([&] {
        done=piddrive(12,1000);
    });
    intakein();
    pros::delay(500);
    //stop move to prevent ovelap with jiggle
    move2.suspend();
    
    //delete thread just in case
    //jiggle to get blocks in
    for(int i=0;i<2;i++) {
    done=piddrive(4, 800);
    pros::delay(120);
    done=piddrive(-2,800);
    pros::delay(100);
    }
    //report that jiggle is done
    master.clear();
    pros::delay(50);
    master.print(1, 0, "Done shaking");
    pros::delay(500);
    //stop intake
    
    //just in case
    waittildone();
    //Backup from matchloader
    done=piddrive(-10, 1700);
    intakestop();
    //retract pneumatic
    matchloader.retract();
    //turn to face diagonally across the square, wait for turn to finish
    robot->turnToHeading(225, 1500,{.direction=AngularDirection::CW_CLOCKWISE});
    waittildone();
    //drive across square
    piddrive(24*sqrt(2)-0.5, 1600);
    //turn to face 270, MAY NEED TO CHANGE
    robot->turnToHeading(267, 1300,{.direction=AngularDirection::CW_CLOCKWISE});
    waittildone();
    //drive across to other side
    piddrive(53.0/2.0,1600);
    robot->turnToHeading(270,800);
    waittildone();
    piddrive(53.0/2.0,1800);
    //turn to face diagonally across the otherside square
    robot->turnToHeading(315, 300,{.direction=AngularDirection::CW_CLOCKWISE});
    waittildone();
    //drive across square
    piddrive(30, 1900);
    //turn to face up score to long goal
    robot->turnToHeading(267, 1350,{.direction=AngularDirection::CCW_COUNTERCLOCKWISE});
    waittildone();
    //drive into longgoal, then add small lowspeed drive 
    piddrive(-20, 3000);
    done=0;
    pros::Task delaymd ([&] {
        done=piddrive(-10, 5000,30);
    });
    //up score into long goal for 5 seconds(could be reduced later)
    outup();
    int i=0;
    while (i<2) {
        pros::delay(2000);
        outdown();
        pros::delay(500);
        outup();
        i++;    
    }
    intakestop();
    //extend matchloader facing the other way
    matchloader.extend();
    //wait for it to finish
    pros::delay(200);
    //drive into other mobile goal
    delaymd.suspend();
    done=false;
    done=piddrive(48,2000);

    //after its done do same routine as before, but opposite direction jitter
    intakein();
    for(int i=0;i<2;i++) {
    done=piddrive(-4, 800);
    pros::delay(100);
    done=piddrive(2,800);
    pros::delay(100);
    }
    //report done
    master.clear();
    pros::delay(50);
    master.print(1, 0, "Done shaking");
    pros::delay(500);
    //stop intake, drive back to long-goal
    
    piddrive(-42, 3000);
    //do slow drive into long-goal
    piddrive(-4, 1000,30);
    //up score for 4 secs
    outup();
    pros::delay(4000);
    intakestop();
    //done for now
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
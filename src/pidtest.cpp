#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <string>

void pidtest(lemlib::Drivetrain drivetrain,lemlib::OdomSensors sensors,lemlib::ExpoDriveCurve throttle_curve,lemlib::ExpoDriveCurve steer_curve, std::string type){
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    std::string test = "PID Test: " + type;
    master.clear();
    pros::delay(50);
    master.print(0, 0, test.c_str());
    //add stuff later
    //0 is p, 1 is i, 2 is d, 3 is g
    double val[] = {0,0,0,0};
    std::string valstr[]={"p ","i ","d ","goal"};
    double inc[] = {0,0,0,0};
    int s=0;
    //defaults
    if (type=="drive") {
        val[0]=20;
        val[1]=0;
        val[2]=1;
        val[3]=12;
        //increments for p,i,d,goal
        double inc[] = {1,0.1,0.1,1};
        lemlib::ControllerSettings angular_controller(10,0,0.5,0,0,0,0,0,0);
    }
    else if (type=="turn") {
        val[0]=10;
        val[1]=0;
        val[2]=0.5;
        val[3]=90;
        //increments for p,i,d,goal
        double inc[] = {1,0.1,0.1,10};
        lemlib::ControllerSettings lateral_controller(20,0,1,0,0,0,0,0,0);
    }
    bool complete=false;
    bool changed=true;
        while (!complete) {
            //draw gui
            if (changed) {
            std::string valstr="p:"+std::to_string(val[0])+" i:"+std::to_string(val[1])+" d:"+std::to_string(val[2])+" g:"+std::to_string(val[3]);
            std::string selstr="Selected:"+valstr[s];
            master.clear_line(1);
            pros::delay(50);
            master.print(1, 0,valstr.c_str());
            pros::delay(50);
            master.print(2, 0,selstr.c_str());
            pros::delay(20);
            }
            
            //controls
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
                val[s]+=inc[s];
                changed=true;
               
            }
            else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
                val[s]-=inc[s];
                changed=true;
                
            }
            else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
                s=(s+1);
                if (s>3) {
                s=0;
                }
                changed=true;
                
            }
            else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
                s=(s-1);
                if (s<0) {
                s=3;
                }
                changed=true;
                
            }
            else {
                changed=false;
            }
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                complete=true;
                
            }
            
        }
        if (type=="drive") {
            lemlib::ControllerSettings lateral_controller(val[0],val[1],val[2],0,0,0,0,0,0);
            lemlib::Chassis chassis(drivetrain, // drivetrain settings
                                    lateral_controller, // lateral PID settings
                                    lemlib::ControllerSettings(10,0,0.5,0,0,0,0,0,0), // angular PID settings
                                    sensors, // odometry sensors
                                    &throttle_curve,
                                    &steer_curve
            );
            chassis.setPose(0,0,0);
            chassis.moveToPoint(0, val[3], 5);
        }
        else if (type=="turn") {
            lemlib::ControllerSettings angular_controller(val[0],val[1],val[2],0,0,0,0,0,0);
            lemlib::Chassis chassis(drivetrain, // drivetrain settings
                                    lemlib::ControllerSettings(20,0,1,0,0,0,0,0,0), // lateral PID settings
                                    angular_controller, // angular PID settings
                                    sensors, // odometry sensors
                                    &throttle_curve,
                                    &steer_curve
            );
            chassis.setPose(0,0,0);
            chassis.turnToHeading(val[3], 5);
        }

}
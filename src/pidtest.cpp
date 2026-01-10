#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <cmath>
#include <string>

double rtp(double value, int places) {
    double factor = std::pow(10.0, places);
    value = std::round(value * factor) / factor;
    return value;
}
std::string strtrim(double value) {
    std::string valuestr = std::to_string(value);
    valuestr.erase ( valuestr.find_last_not_of('0') + 1, std::string::npos );
    return valuestr;
}
void pidtest(lemlib::Drivetrain drivetrain,lemlib::OdomSensors sensors,lemlib::ExpoDriveCurve throttle_curve,lemlib::ExpoDriveCurve steer_curve, std::string type){
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    std::string test = "PID Test: " + type;
    master.clear();
    pros::delay(50);
    master.print(0, 0, test.c_str());
    //add stuff later
    //0 is p, 1 is i, 2 is d, 3 is g
    double val[] = {0,0,0,0};
    std::string valsstr[]={" p"," i"," d"," goal"};
    double inc[4];
    int s=0;
    //defaults
    if (type=="drive") {
        val[0]=20;
        val[1]=0;
        val[2]=1;
        val[3]=12;
        //increments for p,i,d,goal
        double inc[] = {1,0.1,0.1,1};
    }
    else if (type=="turn") {
        val[0]=10;
        val[1]=0;
        val[2]=0.5;
        val[3]=90;
        //increments for p,i,d,goal
        double inc[] = {1,0.1,0.1,10};
        
    }
    bool complete=false;
    bool changed=true;
        while (!complete) {
            //draw gui
            if (changed) {
            std::string valstr="p:"+strtrim(rtp(val[0],3))+" i:"+strtrim(rtp(val[1],3))+" d:"+strtrim(rtp(val[2],3))+" g:"+strtrim(rtp(val[3],3));
            std::string selstr="Selected:"+valsstr[s];
            master.clear_line(1);
            pros::delay(50);
            master.clear_line(2);
            pros::delay(50);
            master.print(1, 0,valstr.c_str());
            pros::delay(50);
            master.print(2, 0,selstr.c_str());
            pros::delay(50);
            }
            
            //controls
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
                val[s]=val[s]+inc[s];
                changed=true;
               
            }
            else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
                val[s]=val[s]-inc[s];
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
            lemlib::Chassis newchassis(drivetrain, // drivetrain settings
                                    lateral_controller, // lateral PID settings
                                    lemlib::ControllerSettings(10,0,0.5,0,0,0,0,0,0), // angular PID settings
                                    sensors, // odometry sensors
                                    &throttle_curve,
                                    &steer_curve
            );
            newchassis.setPose(0,0,0);
            newchassis.moveToPoint(0, val[3], 5);
        }
        else if (type=="turn") {
            lemlib::ControllerSettings angular_controller(val[0],val[1],val[2],0,0,0,0,0,0);
            lemlib::Chassis newchassis(drivetrain, // drivetrain settings
                                    lemlib::ControllerSettings(20,0,1,0,0,0,0,0,0), // lateral PID settings
                                    angular_controller, // angular PID settings
                                    sensors, // odometry sensors
                                    &throttle_curve,
                                    &steer_curve
            );
            newchassis.setPose(0,0,0);
            newchassis.turnToHeading(val[3], 5);
        }

}
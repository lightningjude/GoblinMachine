#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/widgets/label/lv_label.h"

#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <any>
#include <cmath>
#include <string>
#include <valarray>

float val[] = {0,0,0,0};
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
void prtheading(void* ptr) {
    lemlib::Chassis* bot = (lemlib::Chassis*)ptr;
    lv_obj_t*disp = lv_label_create(lv_screen_active());
    std::string goalstr = "Goal:" + std::to_string(val[3]);
    lv_label_set_text(disp, goalstr.c_str());
    lv_obj_align(disp, LV_ALIGN_CENTER, 0, 30);
    std::string errorstr;
    std::string finalstr;
    float errorval;
    float inittheta = bot->getPose(false).theta+val[3];
    pros::delay(100);
    while (bot->isInMotion()) {
        errorval=inittheta-bot->getPose(false).theta;
        errorstr="\nerror:"+std::to_string(errorval);
        finalstr=goalstr+errorstr;
        lv_label_set_text(disp, "");
        lv_label_set_text(disp, finalstr.c_str());
        pros::delay(50);
    }

}
void prtdrive(void* ptr) {
    lemlib::Chassis* bot = (lemlib::Chassis*)ptr;
    lv_obj_t*disp = lv_label_create(lv_screen_active());
    std::string goalstr = "Goal:" + std::to_string(val[3]);
    lv_label_set_text(disp, goalstr.c_str());
    lv_obj_align(disp, LV_ALIGN_CENTER, 0, 30);
    std::string errorstr;
    std::string finalstr;
    float errorval;
    float inity = bot->getPose(false).y+val[3];
    pros::delay(100);
    while (bot->isInMotion()) {
        errorval=inity-bot->getPose(false).theta;
        errorstr="\nerror:"+std::to_string(errorval);
        finalstr=goalstr+errorstr;
        lv_label_set_text(disp, "");
        lv_label_set_text(disp, finalstr.c_str());
        pros::delay(50);
    }

}
void pidtest(lemlib::Drivetrain drivetrain,lemlib::OdomSensors sensors,lemlib::ExpoDriveCurve throttle_curve,lemlib::ExpoDriveCurve steer_curve, std::string type){
    
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    std::string test = "PID Test: " + type;
    master.clear();
    pros::delay(50);
    master.print(0, 0, test.c_str());
    //add stuff later
    //0 is p, 1 is i, 2 is d, 3 is g
    //float val[] = {0,0,0,0}; this is declared globally
    std::string valsstr[]={" p"," i"," d"," goal"};
    double inc[4];
    int s=0;
    //defaults
    if (type=="drive") {
        val[0]=0.225;
        val[1]=0;
        val[2]=0;
        val[3]=12;
        //increments for p,i,d,goal
        // 0 is p, 1 is i, 2 is d, 3 is goal
        inc[0]=0.01;
        inc[1]=0.05;
        inc[2]=0.01;
        inc[3]=1;
    }
    else if (type=="turn") {
        val[0]=0.225;
        val[1]=0;
        val[2]=0;
        val[3]=90;
        //increments for p,i,d,goal
        // 0 is p, 1 is i, 2 is d, 3 is goal
        inc[0]=0.01;
        inc[1]=0.05;
        inc[2]=0.01;
        inc[3]=10;
    }
    bool complete;
    bool changed = true;
    std::string valstr;
    std::string selstr;
    again:
    complete=false;
        while (!complete) {
            //draw gui
            if (changed) {
            valstr="p:"+strtrim(rtp(val[0],3))+" i:"+strtrim(rtp(val[1],3))+" d:"+strtrim(rtp(val[2],3))+" g:"+strtrim(rtp(val[3],3));
            selstr="Selected:"+valsstr[s];
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
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
                master.clear();
                pros::delay(50);
                master.print(0, 0, test.c_str());
                master.clear_line(1);
                pros::delay(50);
                master.print(1, 0,"Cancelled");
                pros::delay(1000);
                return;
            }
            
        }
        pros::delay(100); //give time to update
        master.clear_line(2);
        pros::delay(50);
        std::string report="Running...";
        master.print(2,0,report.c_str());
        if (type=="drive") {
            lemlib::ControllerSettings angular_contr(0,0,0,0,0,0,0,0,0);
            lemlib::ControllerSettings lateral_contr(val[0],val[1],val[2],0,0,0,0,0,0);
            lemlib::Chassis newchassis(drivetrain, // drivetrain settings
                                    lateral_contr, // lateral PID settings
                                    angular_contr, // angular PID settings
                                    sensors, // odometry sensors
                                    &throttle_curve,
                                    &steer_curve
            );
            lemlib::Chassis* chassisptr2= &newchassis;
            newchassis.setPose(0,0,0);
            pros::Task bruh (prtdrive,(void*)chassisptr2,"print task");
            newchassis.moveToPoint(0, val[3], 5000);
            pros::delay(5000); //give time to update
            newchassis.cancelAllMotions();
            master.clear_line(2);
            pros::delay(50);
            
            
           

        }
        else if (type=="turn") {
            lemlib::ControllerSettings angular_cont(val[0],val[1],val[2],0,0,0,0,0,0);
            lemlib::ControllerSettings lateral_cont(0,0,0,0,0,0,0,0,0);
            lemlib::Chassis newchassis(drivetrain, // drivetrain settings
                                    lateral_cont, // lateral PID settings
                                    angular_cont, // angular PID settings
                                    sensors, // odometry sensors
                                    &throttle_curve,
                                    &steer_curve
            );
            lemlib::Chassis* chassisptr2= &newchassis;
            newchassis.setPose(0,0,0);
            pros::Task bruh (prtheading,(void*)chassisptr2,"print task");
            newchassis.turnToHeading(val[3], 5000);
            pros::delay(5000); //give time to update
            newchassis.cancelAllMotions();
            master.clear_line(2);
            pros::delay(50);
        
            
            
        }
        master.print(2,0,"Done");
        goto again;

}
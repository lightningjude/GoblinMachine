#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"

void pidtest(lemlib::Drivetrain drivetrain,lemlib::OdomSensors sensors,lemlib::ExpoDriveCurve throttle_curve,lemlib::ExpoDriveCurve steer_curve, std::string type){
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    std::string test = "PID Test: " + type;
    master.clear();
    pros::delay(50);
    master.print(0, 0, test.c_str());
    //add stuff later
    double p=0;
    double i=0;
    double d=0;
    double g=0;
    //defaults
    if (type=="drive") {
        p=20;
        i=0;
        d=1;
        g=12;
        lemlib::ControllerSettings angular_controller(10,0,0.5,0,0,0,0,0,0);
    }
    else if (type=="turn") {
        p=10;
        i=0;
        d=0.5;
        g=90;
        lemlib::ControllerSettings lateral_controller(20,0,1,0,0,0,0,0,0);
    }


}
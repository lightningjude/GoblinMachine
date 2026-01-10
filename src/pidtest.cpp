#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"

void pidtest(lemlib::Drivetrain drivetrain,lemlib::OdomSensors sensors,lemlib::ExpoDriveCurve throttle_curve,lemlib::ExpoDriveCurve steer_curve, std::string type){
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    std::string test = "PID Test: " + type;
    master.clear();
    master.p
    //add stuff later

}
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "liblvgl/llemu.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"




//tester for now



double s=0;
double g=12;
double p=20;
double d=1;
double i=0;

//Lemlib setup

//wheel dim: lemlib::Omniwheel::NEW_325
//trackwidth inches
double twi = 16;



// motor config
pros::MotorGroup left_motors({1, 2, 3}, pros::MotorGearset::blue); // left motors use 600 RPM cartridges
pros::MotorGroup right_motors({-4, -5, -6}, pros::MotorGearset::blue); // right motors use 200 RPM cartridges
 
pros::Motor left_front(1, pros::MotorGearset::blue);
//intake motors and pneumatics defined intakefunctions.cpp
//matchload is in intakemain.cpp


// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                              &right_motors, // right motor group
                              twi, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

//inertial sensor
// create an imu on port 10
pros::Imu imu(20);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

//PID setup
// lateral PID controller
lemlib::ControllerSettings lateral_controller(p, // proportional gain (kP)
                                              i, // integral gain (kI)
                                              d, // derivative gain (kD)
                                              0, // anti windup: 3
                                              0, // small error range, in inches: 1
                                              0, // small error range timeout, in milliseconds: 100
                                              0, // large error range, in inches: 3
                                              0, // large error range timeout, in milliseconds: 500
                                              0 // maximum acceleration (slew): 20, but tune last starting from 127 down
);

// angular PID controller
lemlib::ControllerSettings angular_controller(p, // proportional gain (kP)
                                              i, // integral gain (kI)
                                              d, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

//Drive curves

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);
// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
						&throttle_curve,
						&steer_curve

);
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */


void initialize() {
	
	chassis.calibrate();
	const auto [color,colorid,sp,sorm] = gui();
	pros::Task intakethread_task(intakethread);
	//pros::lcd::initialize();
	
	/*pros::Task screen_task([&]() {

		
		while (true) {
            // print robot location to the brain screen
			
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
			master.clear();
			master.print(0,0,"X: %f",chassis.getPose().x);
			master.print(1,0,"Y: %f",chassis.getPose().y);
			master.print(2,0,"Theta: %f",chassis.getPose().theta);
			
            // delay to save resources
            pros::delay(20);
        }
		
    });
	*/
}

/*
button mapping
1. R2(Top Score) 
2. L2(Middle score)
3. L1(Intake to reservoir)
3. Left stick fwd/bwd, r stick l/r
4. R1: ground score(good color send down, bad color descore up)
*/

//variables for later
pros::Color color;
int colorid;
int sp;
int sorm;





/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	//variables for input, color as a color, color as a #(0 for red, 1 for blue), starting position(0 for left, 1 for right), skills or match(0 for match, 1 for skills)
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	
	/*
	Key ideas
	c is the color as pros::Color
	colorid is the color as int(0 for red, 1 for blue)
	sp is starting position(0 for left, 1 for right)
	sorm is skills or match(0 for match, 1 for skills)
	If using paths, pick paths based on these variables, or use them in if statements if doing it by hand instead
	*/
	if (sorm==0) {
		//match
		if (sp==0) {
			//left
			if (colorid==0) {
				//red
			}
			else {
				//blue
			}
		}
		else {
			//right
			if (colorid==0) {
				//red
			}
			else {
				//blue
			}
		}
	}
	else {
		
		//skills
	}
	
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

// main opcontrol
void testgui()	{
	pros::Controller master(pros::E_CONTROLLER_MASTER);
    master.clear();
    master.print(0,0,"Choose type:");
    master.print(1,0,"L: Drive");
    master.print(2,0,"R: Turn");
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            pidtest(drivetrain,sensors,throttle_curve,steer_curve,"drive");
            
        }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            pidtest(drivetrain,sensors,throttle_curve,steer_curve,"turn");
            
        }
        pros::delay(20);
    }
}
void opcontrol() {
	//1 is turn, 0 is drive
	/*
	if (s==1) {
		chassis.setPose(0,0,0);
		chassis.turnToHeading(g, 5);
	}
	else if (s==0) {
		chassis.setPose(0,0,0);
		chassis.moveToPoint(0, g, 5);
	}
		*/
	
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	//comment out when needed
	testgui();
	while (true) {
		

		// Arcade control scheme
		int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		
		//last value is called desaturate bias
		/* desaturateBias has a range of 0 to 1, 
		and only has an effect if motor output would be above 127. 
		If set to 0, steering will be reduced until the motor output is below 127, 
		leaving throttle alone, and vice versa for a value 1. 
		The default is 0.5, where steering and throttle have the same priority.
		*/
		chassis.arcade(leftY, rightX, false, 0.5);
		pros::delay(20);                               // Run for 20 ms then update
	}
	
}
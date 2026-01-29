#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "liblvgl/misc/lv_types.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <new>



//HEYY, replace all tasks(especially in skills) that involve delayed whatver with just that code in main task, as drive tasks run asyn anyway



//tester for now



double s=0;
double g=90;
double p=0;
double d=0;
double i=0;


//Lemlib setup

//wheel dim: lemlib::Omniwheel::NEW_325
//trackwidth inches




// motor config
pros::MotorGroup right_motors({1, 2, 3}, pros::MotorGearset::blue); // left motors use 600 RPM cartridges
pros::MotorGroup left_motors({-4, -5, -6}, pros::MotorGearset::blue); // right motors use 200 RPM cartridges
 
pros::Motor left_front(1, pros::MotorGearset::blue);
//intake motors and pneumatics defined intakefunctions.cpp
//matchload is in intakemain.cpp


// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                              &right_motors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              360, // drivetrain rpm is 360
                              8 // horizontal drift is 2 (for now)
);

//inertial sensor
// create an imu on port 18
pros::Imu imu(18);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to nullptr
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

//PID setup
// lateral PID controller
lemlib::ControllerSettings lateral_controller( 10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup: 3
                                              0, // small error range, in inches: 1
                                              0, // small error range timeout, in milliseconds: 100
                                              0, // large error range, in inches: 3
                                              0, // large error range timeout, in milliseconds: 500
                                              127 // maximum acceleration (slew): 20, but tune last starting from 127 down
);

// angular PID controller
lemlib::ControllerSettings angular_controller(9.7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              66.5, // derivative gain (kD)
                                              1, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
											  //bruhhh
);

//Drive curves

//Use this desmos to see changes

//desmos of curves: https://www.desmos.com/calculator/umicbymbnl

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(6, // joystick deadband out of 127
                                     20, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(6, // joystick deadband out of 127
                                  25, // minimum output where drivetrain will move out of 127
                                  1.018 // expo curve gain
);
// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
						&throttle_curve,
						&steer_curve

);
lemlib::Chassis* chassisptr=&chassis;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */



void initialize() {
	
	chassis.calibrate();
	while (imu.is_calibrating()) {
		pros::delay(20);
	}
	//uncomment below while testing
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
	const auto [color,colorid,sp,sorm,txt] = selectgui();
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

	//auton selector is currently DISABLED, uncomment from the if (sorm=0) { to enable it again

	//it currently just runs the autonskillshand() function


	/*
	Key ideas
	c is the color as pros::Color
	colorid is the color as int(0 for red, 1 for blue)
	sp is starting position(0 for left, 1 for right)
	sorm is skills or match(0 for match, 1 for skills)
	If using paths, pick paths based on these variables, or use them in if statements if doing it by hand instead
	*/
	 /*
	if (sorm==0) {
		//match
		if (sp==0) {
			//left
			if (colorid==0) {
				//red
				autonlr(chassisptr);
			}
			else {
				//blue
				autonlb(chassisptr);
			}
		}
		else {
			//right
			if (colorid==0) {
				//red
				autonrr(chassisptr);
			}
			else {
				//blue
				autonrb(chassisptr);
			}
		}
	}
	else {
		autonskills(chassisptr);
		//skills
	}
	*/

	//make edits under the autons cpp file

	pros::Controller master(pros::E_CONTROLLER_MASTER);
	master.print(0,0,"auton happened");
	autonskillshand(chassisptr);
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
	pros::delay(50);
    master.print(0,0,"Choose type");
	pros::delay(50);
    master.print(1,0,"L: Drive");
	pros::delay(50);
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
	master.clear();
	master.print(0,0,"Done");
	pros::delay(50);

}

ASSET(skillsp1_txt);
void opcontrol() {
	autonskillshand(chassisptr);
	lv_obj_t* txt;
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	//comment out when needed
	/*
	if (!pros::competition::is_connected())
	{
		master.clear();
		pros::delay(50);
		master.print(0,0,"auton select?");
		pros::delay(50); 
		master.print(1, 0,"y to confirm"); 
		pros::delay(50);
		master.print(2,0,"x to skip");
		bool chosenn=false;
		while (!chosenn) {
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
				chosenn=true;
				const auto [color,colorid,sp,sorm,txt] = selectgui();
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
				chosenn=true;
			}
			pros::delay(20);
		}
		master.clear();
	}
	pros::delay(50);
	while (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)|master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
		pros::delay(20);
	}
	master.clear();
	*/
	/*
	pros::delay(50);
	master.print(0,0,"PID test?");
	pros::delay(50); 
	master.print(1, 0,"y to confirm"); 
	pros::delay(50);
	master.print(2,0,"x to skip");
	bool chosen=false;
	while (!chosen) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			chosen=true;
			testgui();
		}
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
			chosen=true;
		}
		pros::delay(20);
	}
		*/
	
	

	pros::delay(50);
	master.clear();

	bool reversed=false;
	bool revlatch=false;
	delete txt;
	
	//this checks the chassis is actually setting position right

	/*
	lemlib::Pose bruhbefore(0,0,0);
	chassis.setPose(bruhbefore);
	lemlib::Pose bruhafter= chassis.getPose();
	if (bruhafter.x==bruhbefore.x|bruhafter.y==bruhbefore.y|bruhbefore.theta==bruhafter.theta) {
		master.clear();
		pros::delay(50);
		master.print(1, 1, "Poses =");
	}
	else {
		master.clear();
		pros::delay(50);
		master.print(1, 1, "Poses not =");
	}
	*/
	
	//templates for moving backward and forward 12 inches, the .forwards matters
	//chassis.setPose(0,0,0);
	//chassis.moveToPose(0, -12,0, 10000,{.forwards=false});
	//chassis.moveToPose(0,12,0,10000,{.forwards=true});

	//how to turn 90 degrees clockwise
	//chassis.setPose(0,0,0);
	//chassis.moveToPose(0, 0, 90, 10000);

	// this is used to report final position afterward
	/*
	while (chassis.isInMotion()) {
		lemlib::Pose after2 = chassis.getPose();
		pros::delay(50);
		master.clear();
		pros::delay(50);
		master.print(0, 0, "Poses X:%f",after2.x);
		pros::delay(50);
		master.print(1, 0, "Y:%f",after2.y);
		pros::delay(50);
		master.print(2, 0, "theta:%f",after2.theta);
		pros::delay(20);
	} 
	*/
	//autonlb(chassisptr);
	while (true) {
	
		// Arcade control scheme
		int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		int negleftY = -leftY;
		int negrightX = -rightX;
		//last value is called desaturate bias
		/* desaturateBias has a range of 0 to 1, 
		and only has an effect if motor output would be above 127. 
		If set to 0, steering will be reduced until the motor output is below 127, 
		leaving throttle alone, and vice versa for a value 1. 
		The default is 0.5, where steering and throttle have the same priority.
		*/
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
			if (!revlatch) {
				reversed=!reversed;
				revlatch=true;
			}
		}
		else {
			revlatch=false;
		}
		if (reversed) {
			chassis.arcade(leftY, rightX, false, 0.5);
		}
		else {
			chassis.arcade(negleftY, rightX, false, 0.5);
		}
		pros::delay(20);                               // Run for 20 ms then update
	}
	
}
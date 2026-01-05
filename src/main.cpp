#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/misc.h"




//tester for now
pros::Controller master(pros::E_CONTROLLER_MASTER);



//tester gui
std::tuple<double,double,double,double> tester(double g,double p, double d, double i) {
	double val[4]= {g,p,d,i};
	double inc[4]= {10,1,1,1};
	std::string name[4]= {"goal","kp","kd","ki"} ;
	bool going = false;
	int sel = 0;
	bool changed = false;
	master.clear();
	master.print(0,0,"goal: %d",val[0]);
	master.print(1, 0, "kp:%.2,kd:%.2,ki:%.2",val[1],val[2],val[3]);
	master.print(2,0,"selected: %d",name[sel]);
	while (going==false) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
			going=true;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
			if (sel<3) {
				sel++;
			}
			else {
				sel=0;
			}
			changed=true;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)){
			if (sel>0) {
				sel--;
			}
			else {
				sel=3;
			}
			changed=true;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
			val[sel]=val[sel]+inc[sel];
			changed=true;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
			val[sel]=val[sel]-inc[sel];
			changed=true;
		}
		if (changed) {
			master.clear();
			master.print(0,0,"goal: %d",val[0]);
			master.print(1, 0, "kp:%.2,kd:%.2,ki:%.2",val[1],val[2],val[3]);
			master.print(2,0,"selected: %d",name[sel]);
			changed = false;
		}
		pros::delay(20);
	}
	return {val[0],val[1],val[2],val[3]};
}


std::tuple<int,double,double,double,double> gettest() {
	double gldef = 12;
	double pldef = 30;
	double dldef = 1;
	double ildef = 0;

	double gtdef = 90;
	double ptdef = 30;
	double dtdef = 1;
	double itdef = 0;

	
	master.clear();
	master.print(1, 0, "Left for turn, right for drive");
	bool press = false;
	// 1 is drive, 0 is turn, 2 will error
	int dort = 2;
	while (press==false) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			//turn
			dort=0;
			press=true;
		}
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			//drive
			dort=1;
			press=true;
		}
		pros::delay(20);
	}
	if (dort==1) {
		const auto [g,p,d,i] = tester(gldef,pldef,dldef,ildef);
		return {dort,g,p,d,i};
	}
	else if (dort==0) {
		const auto [g,p,d,i] = tester(gtdef,ptdef,dtdef,itdef);
		return {dort,g,p,d,i};
	}
	return {0,0,0,0,0};
}
const auto [s,g,p,d,i] = gettest();




//Lemlib setup

//wheel dim: lemlib::Omniwheel::NEW_325
//trackwidth inches
double twi = 16;



// motor config
pros::MotorGroup left_motors({1, 2, 3}, pros::MotorGearset::blue); // left motors use 600 RPM cartridges
pros::MotorGroup right_motors({-4, -5, -6}, pros::MotorGearset::blue); // right motors use 200 RPM cartridges
 
//intake motors and pneumatics defined in main.h
//matchload is here tho
pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);

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
	
	/** 
	pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
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

void intakethread() {
	bool ttog = false;
	bool tlatch = false;
	bool mtog = false;
	bool mlatch = false;
	bool itog = false;
	bool ilatch = false;
	bool dtog = false;
	bool dlatch = false;
	bool otog = false;
	bool olatch = false;
	bool mltog = false;
	bool mllatch = false;
	while (true) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			if (!ilatch) {
				if (itog) {
					intakein();
				}
				else {
					intakestop();
				}
				itog = !itog;
				mtog=false;
				dtog=false;
				otog=false;
				ilatch = true;
			}
		}
		else {
			ilatch = false;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			if (!mlatch) {
				if (mtog) {
					outmiddle();
				}
				else {
					intakestop();
				}
				mtog = !mtog;
				itog=false;
				dtog=false;
				otog=false;
				mlatch = true;
			}
		}
		else {
			mlatch = false;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			if (!dlatch) {
				if (dtog) {
					outdown();
				}
				else {
					intakestop();
				}
				dtog = !dtog;
				itog=false;
				mtog=false;
				otog=false;
				dlatch = true;
			}
		}
		else {
			dlatch = false;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			if (!olatch) {
				if (otog) {
					if (mtog) {
						outmiddle();
					}
					else if (dtog) {
						outdown();
					}
					else if (itog) {
						intakein();
					}
					else if (ttog) {
						outup();
					}
				}
				else {
					intakestop();
				}
				otog = !otog;
				olatch = true;
			}
		}
		else {
			olatch = false;
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
			if (!mlatch) {
				if (mltog) {
					matchload.retract();
				}
				else {
					matchload.extend();
				}
				mltog = !mltog;
				mllatch = true;
			}
		}
		else {
			mllatch = false;
		}
		pros::delay(20);
	}
}



/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous() {}

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
	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

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

#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "liblvgl/llemu.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"



void intakethread() {
    pros::adi::Pneumatics matchload=pros::adi::Pneumatics('a',true);
    pros::Controller master(pros::E_CONTROLLER_MASTER);
	lv_obj_t* intaketxt;
	intaketxt = lv_label_create(lv_screen_active());
	lv_label_set_text(intaketxt, "Intake Thread Running");
	lv_obj_align(intaketxt, LV_ALIGN_TOP_LEFT, 0, 0);
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
			if (ilatch==false) {
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
			if (mlatch==false) {
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
			if (dlatch==false) {
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
			if (olatch==false) {
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
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
			if (mllatch==false) {
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
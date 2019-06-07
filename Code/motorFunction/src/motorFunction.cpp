//============================================================================
// Name        : motorFunction.cpp
// Author      : Leonardo F. L. S. dos Santos
// Version     :
// Copyright   : use com moderação
// Description :
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>

//#include "BlackLib/BlackMutex/BlackMutex.h"
#include "BlackLib/BlackPWM/BlackPWM.h"
#include "BlackLib/BlackGPIO/BlackGPIO.h"

#include <pthread.h>
#include <syslog.h>

using namespace std;
using namespace BlackLib;

BlackPWM motorRight(P8_13);
BlackPWM motorLeft(P8_19);

// ponte H, pinos de direção de rotação
BlackGPIO IN1(GPIO_65, output, SecureMode); // motor dir
BlackGPIO IN2(GPIO_27, output, SecureMode);

BlackGPIO IN3(GPIO_47, output, SecureMode); // motor esq
BlackGPIO IN4(GPIO_46, output, SecureMode);

bool stopFlag = false;
bool securityFlag = false;
bool freeFlag = true;

#define periodmisec 1000
#define MAX_LEFT 65
#define MAX_RIGHT 110
#define MAX_FORWARD 255
#define MAX_BACKWARD -225

int deadState = 0; //(MAX_FORWARD + MAX_BACKWARD)/ 2;
int straightness = 88;    //(MAX_RIGHT + MAX_LEFT)/2;

void setupMotors() {

	//Setting PWM
	motorRight.setPeriodTime(periodmisec, microsecond);
	motorLeft.setPeriodTime(periodmisec, microsecond);
	motorRight.setPolarity(straight);
	motorLeft.setPolarity(straight);

	//Setting GPIO
	IN1.setValue(low);
	IN2.setValue(low);
	IN3.setValue(low);
	IN4.setValue(low);
}


void goStraight(int throttle) { //function to move forward or backward

	if(throttle > deadState){
		float mRightPercent = (throttle - deadState) / (MAX_FORWARD - deadState);
		float mLeftPercent = (throttle - deadState) / (MAX_FORWARD - deadState);

		motorRight.setDutyPercent(mRightPercent);
		IN1.setValue(high);
		IN2.setValue(low);

		motorLeft.setDutyPercent(mLeftPercent);
		IN3.setValue(low);
		IN4.setValue(high);
	}
	if (throttle < deadState){
		float mRightPercent = (deadState- throttle) / (deadState - MAX_BACKWARD);
		float mLeftPercent = (deadState- throttle) / (deadState - MAX_BACKWARD);

		motorRight.setDutyPercent(mRightPercent);
		IN1.setValue(low);
		IN2.setValue(high);

		motorLeft.setDutyPercent(mLeftPercent);
		IN3.setValue(high);
		IN4.setValue(low);
	}
	if (throttle == deadState){

		motorRight.setDutyPercent(0.0);
		IN1.setValue(high);
		IN2.setValue(high);

		motorLeft.setDutyPercent(0.0);
		IN3.setValue(high);
		IN4.setValue(high);
	}
}

void turnSide(int handle) { //function to turn Left or turn Right

	if(handle > straightness){
		float mRightPercent = (handle - straightness) / (MAX_RIGHT - straightness);
		float mLeftPercent = 1 - mRightPercent;

		motorRight.setDutyPercent(mRightPercent);
		IN1.setValue(high);
		IN2.setValue(low);

		motorLeft.setDutyPercent(mLeftPercent);
		IN3.setValue(low);
		IN4.setValue(high);
	}
	if(handle < straightness) {
		float mRightPercent = (straightness - handle) / (straightness - MAX_LEFT);
		float mLeftPercent = 1 - mRightPercent;

		motorRight.setDutyPercent(mRightPercent);
		IN1.setValue(low);
		IN2.setValue(high);

		motorLeft.setDutyPercent(mLeftPercent);
		IN3.setValue(high);
		IN4.setValue(low);
	}
}

/*
int main() {

	setupMotors();

	goStraight(180);

	cout << motorRight.getValue() << endl; // prints

	return 0;
}
*/

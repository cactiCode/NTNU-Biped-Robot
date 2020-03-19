/*
Created by Martin Skårerverket
Original creation date: March 18, 2020

Development code for implementation of LSM9DS1 chip.
Bachelors thesis spring 2020, NTNU

*/
#include <iostream>
#include <string>
#include <unistd.h>
#include "LSM9DS1.h"

using namespace std;


int main(int argc, char* argv[]) {


	

	// Accelerometer Test

	LSM9DS1 accelerometer(2, 0x6b);
	accelerometer.setRange(PLUSMINUS_2g);
	accelerometer.setODR(XL_ODR_952);

	cout << "The current ODR is: " << (int)accelerometer.getODR() << endl;
	cout << "The current range is: " << (int)accelerometer.getRange() << endl;

	while (1 < 2) {

		accelerometer.readFullSensorState();
		int x = accelerometer.getAccelerationX();
		int y = accelerometer.getAccelerationY();
		int z = accelerometer.getAccelerationZ();
		cout << "Current Acceleration: " << x << "," << y << "," << z << endl;

	}




	return 0;
}
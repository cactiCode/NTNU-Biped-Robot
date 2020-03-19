/*
Created by Martin Skårerverket
Original creation date: March 18, 2020

Development code for implementation of LSM9DS1 chip.
Bachelors thesis spring 2020, NTNU

*/

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include "LSM9DS1.h"


using namespace std;

#define MAX_BUS 64

#define ACC_X_LSB           0x28   
#define ACC_X_MSB           0x29	        
#define ACC_Y_LSB           0x2A 	
#define ACC_Y_MSB 	        0x2B
#define ACC_Z_LSB 	        0x2C
#define ACC_Z_MSB           0x2D

#define RANGE               0x20
#define ODR	   	            0x20



LSM9DS1::LSM9DS1(int bus, int address)
{
    I2CBus = bus;
    I2CAddress = address;
    readFullSensorState();
}

void LSM9DS1::calculatePitchAndRoll()
{
    double accelerationXSquared = this->accelerationX * this->accelerationX;
    double accelerationYSquared = this->accelerationY * this->accelerationY;
    double accelerationZSquared = this->accelerationZ * this->accelerationZ;

    this->pitch = 180 * atan(accelerationX / sqrt(accelerationYSquared + accelerationZSquared)) / M_PI;
    this->roll = 180 * atan(accelerationY / sqrt(accelerationXSquared + accelerationZSquared)) / M_PI;
}

int LSM9DS1::readFullSensorState()
{
    char namebuf[MAX_BUS];
    snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0) {
        cout << "Failed to open LSM9DS1 Sensor on " << namebuf << " I2C Bus" << endl;
        return(1);
    }
    if (ioctl(file, I2C_SLAVE, I2CAddress) < 0) {
        cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
        return(2);
    }

    // Might not be neccesary for this chip..
    char buf[1] = { 0x00 };
    if (write(file, buf, 1) != 1) {
        cout << "Failed to Reset Address in readFullSensorState() " << endl;
    }

    int numberBytes = LSM9DS1_I2C_BUFFER;
    int bytesRead = read(file, this->dataBuffer, numberBytes);
    if (bytesRead == -1) {
        cout << "Failure to read Byte Stream in readFullSensorState()" << endl;
    }
    close(file);

    //if (this->dataBuffer[0] != 0x03) {
     //   cout << "MAJOR FAILURE: DATA WITH LSM9DS1 HAS LOST SYNC!" << endl;
    //}

    this->accelerationX = convertAcceleration(ACC_X_MSB, ACC_X_LSB);
    this->accelerationY = convertAcceleration(ACC_Y_MSB, ACC_Y_LSB);
    this->accelerationZ = convertAcceleration(ACC_Z_MSB, ACC_Z_LSB);
    this->calculatePitchAndRoll();

    return 0;
}

int LSM9DS1::convertAcceleration(int msb_reg_addr, int lsb_reg_addr) {
    //	cout << "Converting " << (int) dataBuffer[msb_reg_addr] << " and " << (int) dataBuffer[lsb_reg_addr] << endl;;
    short temp = dataBuffer[msb_reg_addr];
    temp = (temp << 8) | dataBuffer[lsb_reg_addr];
    temp = temp >> 2;
    temp = ~temp + 1;
    //	cout << "The X acceleration is " << temp << endl;
    return temp;
}

void LSM9DS1::displayMode(int iterations) {

    for (int i = 0; i < iterations; i++) {
        this->readFullSensorState();
        printf("Rotation (%d, %d, %d)", accelerationX, accelerationY, accelerationZ);
    }
}

LSM9DS1_RANGE LSM9DS1::getRange() {
    this->readFullSensorState();
    char temp = dataBuffer[RANGE];
    //char temp = this->readI2CDeviceByte(RANGE);  //bits 4,5
    temp = temp & 0b00110000;
    temp = temp >> 1;
    //cout << "The current range is: " << (int)temp << endl;
    this->range = (LSM9DS1_RANGE)temp;
    return this->range;
}

int LSM9DS1::setRange(LSM9DS1_RANGE range) {
    //char current = this->readI2CDeviceByte(RANGE);  //bits 3,2,1
    this->readFullSensorState();
    char current = dataBuffer[RANGE];
    char temp = range << 3; //move value into bits 3,2,1
    current = current & 0b1101111; //clear the current bits 5,4
    temp = current | temp;
    if (this->writeI2CDeviceByte(RANGE, temp) != 0) {
        cout << "Failure to update RANGE value" << endl;
        return 1;
    }
    return 0;
}

LSM9DS1_ODR LSM9DS1::getODR() {

    this->readFullSensorState();
    char temp = dataBuffer[ODR];   //bits 7->4
    //char temp = this->readI2CDeviceByte(BANDWIDTH);  //bits 7,6,5,
//	cout << "The value of bandwidth returned is: " << (int)temp << endl;
    temp &= 0x1F; // 0b00011111
    //temp = temp >> 3;
    //	cout << "The current bandwidth is: " << (int)temp << endl;
    this->odr_config = (LSM9DS1_ODR)temp;
    return this->odr_config;
}

int LSM9DS1::setODR(LSM9DS1_ODR odr_config) {

    this->readFullSensorState();
    char current = dataBuffer[ODR];   //bits 7->4
    char temp = odr_config << 5; //move value into bits 7,6,5,4
    //current = current & 0b00011111; //clear the current bits 7,6,5,4
    //temp = current | temp;
    //uint8_t temp = ;

    if (this->writeI2CDeviceByte(ODR, temp) != 0) {
        cout << "Failure to update ODR value" << endl;
        return 1;
    }
    return 0;
}

int LSM9DS1::writeI2CDeviceByte(char address, char value) {

    //cout << "Starting BMA180 I2C sensor state write" << endl;
    char namebuf[MAX_BUS];
    snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0) {
        cout << "Failed to open LSM9DS1 Sensor on " << namebuf << " I2C Bus" << endl;
        return(1);
    }
    if (ioctl(file, I2C_SLAVE, I2CAddress) < 0) {
        cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
        return(2);
    }

    char buffer[2];
    buffer[0] = address;
    buffer[1] = value;
    if (write(file, buffer, 2) != 2) {
        cout << "Failure to write values to I2C Device address." << endl;
        return(3);
    }
    close(file);
    //cout << "Finished BMA180 I2C sensor state write" << endl;
    return 0;

}

char LSM9DS1::readI2CDeviceByte(char address) {
    //  cout << "Starting BMA180 I2C sensor state byte read" << endl;
    char namebuf[MAX_BUS];
    snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0) {
        cout << "Failed to open LSM9DS1 Sensor on " << namebuf << " I2C Bus" << endl;
        return(1);
    }
    if (ioctl(file, I2C_SLAVE, I2CAddress) < 0) {
        cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
        return(2);
    }
    // According to the BMA180 datasheet on page 59, you need to send the first address
    // in write mode and then a stop/start condition is issued. Data bytes are
    // transferred with automatic address increment.
    char buf[1] = { 0x00 };
    if (write(file, buf, 1) != 1) {
        cout << "Failed to Reset Address in readFullSensorState() " << endl;
    }
    char buffer[1];
    buffer[0] = address;
    if (read(file, buffer, 2) != 2) {
        cout << "Failure to read value from I2C Device address." << endl;
    }
    close(file);

    return buffer[0];
}



LSM9DS1::~LSM9DS1() {

}



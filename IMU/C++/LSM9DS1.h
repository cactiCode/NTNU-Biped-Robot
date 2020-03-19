/*
Created by Martin Skårerverket
Original creation date: March 18, 2020

Development code for implementation of LSM9DS1 chip.
Bachelors thesis spring 2020, NTNU

*/

#define LSM9DS1_H_
#define LSM9DS1_I2C_BUFFER 0x80

enum LSM9DS1_RANGE
{
    // Defines possible ranges of the accelerometer

    PLUSMINUS_2g,
    PLUSMINUS_16g,
    PLUSMINUS_4g,
    PLUSMINUS_8g
};

enum LSM9DS1_ODR
{
    //Defines possible data  output rates of the accelerometer.
    //By default Bandwidth is determined by the ODR, This can be changed in reg BW_XL, see Datasheet

    XL_POWER_DOWN,      // Power- down mode 
    XL_ODR_10,          // Data rate = 10Hz | BW = 408Hz
    XL_ODR_50,          // Data rate = 50Hz | BW = 408Hz
    XL_ODR_119,         // Data rate = 119Hz | BW = 50Hz 
    XL_ODR_238,         // Data rate = 238Hz | BW = 105Hz
    XL_ODR_476,         // Data rate = 476Hz | BW = 211Hz
    XL_ODR_952          // Data rate = 952HZ | BW = 408Hz
};

class LSM9DS1
{

private:

    int I2CBus, I2CAddress;
    char dataBuffer[LSM9DS1_I2C_BUFFER];

    int accelerationX;
    int accelerationY;
    int accelerationZ;

    double pitch;
    double roll;


    LSM9DS1_RANGE range;
    LSM9DS1_ODR odr_config;

    int convertAcceleration(int msb_addr, int lsb_addr);
    int writeI2CDeviceByte(char address, char value);
    char readI2CDeviceByte(char address);

    void calculatePitchAndRoll();


public:

    LSM9DS1(int bus, int address);

    void displayMode(int iterations);

    int readFullSensorState();

    int setRange(LSM9DS1_RANGE range);
    LSM9DS1_RANGE getRange();

    int setODR(LSM9DS1_ODR odr_config);
    LSM9DS1_ODR getODR();


    int getAccelerationX() { return accelerationX; }
    int getAccelerationY() { return accelerationY; }
    int getAccelerationZ() { return accelerationZ; }

    int getPitch() { return pitch; }
    int getRoll() { return roll; }

    virtual ~LSM9DS1();

};


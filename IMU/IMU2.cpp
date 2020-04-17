#include <iostream>
#include <string>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stropts.h>

#define g_sens_accs 0.000732 //value from datasheet page 12
#define PI 3.14159 // constant pi




class LSM9DS1
{
    private :
    int file;
    char addr;
    float ax,ay,az;
    float pitch,roll;

    char dataBuffer[5] = {0};

    void convertData()
    {
        ax = ((dataBuffer[1] << 8) | dataBuffer[0])*g_sens_accs; // Store x-axis values into ax
        ay = ((dataBuffer[3] << 8) | dataBuffer[2])*g_sens_accs; // Store y-axis values into ay
        az = ((dataBuffer[5] << 8) | dataBuffer[4])*g_sens_accs; // Store z-axis values into az
        
        roll = atan2(ay, az);
        pitch = atan2(-ax, sqrt(ay * ay + az * az));

        //convert from radians to degrees
        pitch *= 180.0 / PI;
        roll  *= 180.0 / PI;
        
        // print converted axis values
        printf("X-axis: %f \n", ax);
        printf("Y-axis: %f \n", ay);
        printf("Z-axis: %f \n", az);

        printf("roll : %f \n", roll);
        printf("pitch : %f \n", pitch);

    }


    public :

    LSM9DS1(char address)
    {
        addr = address;
    }
    void setRegSensor(char reg,char value)
    {
        char *bus ="/dev/i2c-2";
        //Open I2C bus, checks connection
        if((file = open(bus, O_RDWR)) < 0)
        {
            printf("Failed to open bus. \n");
            exit(1);
        }
        ioctl(file, I2C_SLAVE, addr);

        char config[2] = {0};
        config[0] = reg;
        config[1] = value;

        write(file, config, 2);
        printf("sucessful init \n");

    }
    void ReadSensor()
    {
        char regis[1] = {0x28};
        write(file, regis, 1);
    
        if(read(file, dataBuffer, 6) != 6)
        {
            printf("Input/Output error \n");
            exit(1);
        }
        convertData();
    }

    void closeSensor()
    {
        close(file);
    }

};

int main(int argc, char* argv[])
{
    char addresse = 0x6b;
    LSM9DS1 acceleration(addresse);
    char aksereg = 0x20;
    char setBit  = 0xc0;
    acceleration.setRegSensor(aksereg,setBit);
    int count = 0;

    while(count < 10)
    {
        acceleration.ReadSensor();
        count++;
    }
    acceleration.closeSensor();

    std::cin.get();
    return 0;
}



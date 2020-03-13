#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>

#define g_sens_accs 0.000732 //value from datasheet page 12
#define PI 3.14159 // constant pi

void main()
{
    //Create I2C bus
    int file;
    float ax,ay,az;
    float pitch,roll;



    char *bus ="/dev/i2c-2";
    //Open I2C bus, checks connection
    if((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open bus. \n");
        exit(1);
    }
    
    //Select the i2c adress(0x6b)
    ioctl(file, I2C_SLAVE, 0x6b);

    while(1){
    
    //Select Linear acceleration sensor Control Register(0x20), enable x,y,z, axis.
    //Output Data Rate = 952hz. Accl scale = +-16g, BW = 408hz (0xC8)
    char config[2] = {0};
    config[0] = 0x20;
    config[1] = 0xC8;

    write(file, config, 2);

    

    //Read data from linear accel. starting from address (0x28) to (0x2D) total 6 bytes.
    char reg[1] = {0x28};
    write(file, reg, 1);

    
    char data[5] = {0};
    
    if(read(file, data, 6) != 6)
    {
        printf("Input/Output error \n");
        exit(1);
    }

    // convert data from accel register to x,y and z-axis
    ax = ((data[1] << 8) | data[0])*g_sens_accs; // Store x-axis values into ax
	ay = ((data[3] << 8) | data[2])*g_sens_accs; // Store y-axis values into ay
	az = ((data[5] << 8) | data[4])*g_sens_accs; // Store z-axis values into az
    
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








}
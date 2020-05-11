#include <iostream>
#include <unistd.h>
#include <cmath>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define g_sens_accs 0.061 //value from datasheet page 12
#define PI 3.14159 // constant pi




class LSM9DS1
{
    private :
    int file;
    char addr;
    float ax,ay,az;
    short s_ax,s_ay,s_az;
    float pitch,roll;

    char dataBuffer[5] = {0};

    void convertData()
    {
        s_ax = ((dataBuffer[1] << 8) | dataBuffer[0]); 
        s_ay = ((dataBuffer[3] << 8) | dataBuffer[2]);
        s_az = ((dataBuffer[5] << 8) | dataBuffer[4]);
        ax = (s_ax*g_sens_accs)/1000;
        ay = (s_ay*g_sens_accs)/1000;
        az = (s_az*g_sens_accs)/1000;
        
        roll = atan2(ay,az);
        pitch = atan2(-ax, sqrt(ay * ay + az * az));

        //convert from radians to degrees
        pitch *= 180.0 / PI;
        roll  *= 180.0 / PI;
        
        // print converted axis values
        printf("\033[2J\033[1;1H");
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
        if(write(file, config, 2) < 0)
        {
            printf("Failed to open bus. \n");
            exit(1);
        }
        else
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
    }
    acceleration.closeSensor();

    std::cin.get();
    return 0;
}

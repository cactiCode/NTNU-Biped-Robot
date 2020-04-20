#include "ros/ros.h"
#include "std_msgs/String.h"

#include <iostream>
#include <sstream>
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
    }

    public :
    LSM9DS1(char address)
    {
        addr = address;
    }
    void setRegSensor(char reg,char value)
    {
        char *bus ="/dev/i2c-2";
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

    float GetAccelerationX(){ return ax;}
    float GetAccelerationY(){ return ay;}
    float GetAccelerationZ(){ return az;}
};

int main(int argc, char **argv)
{
    //ROS code
    ros::init(argc,argv,"IMUpublisher");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Rate loop_rate(100);

    //IMU code 
    char addresse = 0x6b;
    LSM9DS1 acceleration(addresse);
    char aksereg = 0x20;
    char setBit  = 0xc0;
    acceleration.setRegSensor(aksereg,setBit);
    int count = 0;

    while (ros::ok())
    {
        std_msgs::String msg;
        std::stringstream ss;
        acceleration.ReadSensor();
        ss << std::endl; 
        ss << "AccelX : " << acceleration.GetAccelerationX() << std::endl;
        ss << "AccelY : " << acceleration.GetAccelerationY() <<std::endl;
        ss << "AccelZ : " << acceleration.GetAccelerationZ() << std::endl;
        ss << "[ Count ] : " << count << std::endl; 
        msg.data = ss.str();
        ROS_INFO("%s", msg.data.c_str());

        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
}



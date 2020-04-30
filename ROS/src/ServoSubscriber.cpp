#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <fstream>
#include <string>

class servoControl
{


private:

    int m_frequency = 20006000;
    int m_dutyCycle;
    int m_PIN;
    int m_pinbool;
    std::string m_Pinpath;
    std::string m_PinpathNumber;
    std::string m_pinChip;
    
    
    void setPin(int pin)
    {  
        switch (pin) 
        {
            case 13:
                m_Pinpath = "P8_13_pinmux";
                m_PinpathNumber = "7:1";
                m_pinChip = "pwmchip7";
                m_pinbool = 1;
                break;
            case 19:
                m_Pinpath = "P8_19_pinmux";
                m_PinpathNumber = "7:0";
                m_pinChip = "pwmchip7";
                m_pinbool = 0;
                break;
            case 29:
                m_Pinpath = "P9_29_pinmux";
                m_PinpathNumber = "1:1";
                m_pinChip = "pwmchip1";
                m_pinbool = 1;
                break;
            case 31:
                m_Pinpath = "P9_31_pinmux";
                m_PinpathNumber = "1:0";
                m_pinChip = "pwmchip1";
                m_pinbool = 0;
                break;
            default:
                std::cout << "Not an implemented pin"<<std::endl;
                break;
        }
    }

    void setPWMstate()
    {
        std::ofstream pwmstate;
        pwmstate.open("/sys/devices/platform/ocp/ocp:" + m_Pinpath + "/state");
        //få inn feiltest
        pwmstate << "pwm";
        pwmstate.close();
    }

    void enablePWM()
    {
        std::ofstream pwmEnable;
        pwmEnable.open("/sys/class/pwm/pwm-" + m_PinpathNumber + "/enable");
        pwmEnable << 1;
        pwmEnable.close();
    }
    void setPWMfreq(int frequency)
    {
        std::ofstream PWMexportStream;
        PWMexportStream.open("/sys/class/pwm/" + m_pinChip + "/export");
        PWMexportStream << m_pinbool;
        PWMexportStream.close();

        std::ofstream freqSet;
        freqSet.open("/sys/class/pwm/pwm-" + m_PinpathNumber + "/period");
        freqSet << frequency;
        freqSet.close();
    }

public:
     servoControl(int pinNumber)
    {
        m_dutyCycle = 1000000;
        setPin (pinNumber);
        std::cout<< "Running with pin " << pinNumber << std::endl;
    }
    void setDutyCycle(int dutyC)
    {
        std::ofstream DCfile;
        DCfile.open("/sys/class/pwm/pwm-" + m_PinpathNumber + "/duty_cycle");
        DCfile << dutyC;
        DCfile.close();
    }

    void servoInit()
    {   
        setPWMstate();
        setPWMfreq(m_frequency);
        setDutyCycle(m_dutyCycle);
        enablePWM();
        
    }
};
    //servocode
    servoControl servoA(31); //initalize with pin 19
    servoControl servoB(19); //initalize with pin 19
    
    

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    //ros code
    ROS_INFO("servo_recive : [%s]", msg->data.c_str());
    std::string servo_pos = msg->data;
    std::cout << servo_pos <<std::endl;
    //servo code
    if (servo_pos == "0")
    {
        servoA.setDutyCycle(1900000);
        servoB.setDutyCycle(1900000);
    }
    else if(servo_pos == "1" )
    {
        servoA.setDutyCycle(820000);
        servoB.setDutyCycle(820000);
    }
    else 
    {
        servoA.setDutyCycle(1000000);
        servoB.setDutyCycle(1000000);
    }
}

int main(int argc, char **argv)
{
    //servo code
    servoA.servoInit();
    servoB.servoInit();
    //ros code
    ros::init(argc, argv, "Servo_subscriber");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    ros::spin();
    return 0;
}

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



int main()
{
    int pinN,pinN2;
    std::cout << "Pin Nr :";
    std::cin >> pinN;
    std::cout << std::endl;
    std::cout << "Pin2 Nr :";
    std::cin >> pinN2;
    std::cout << std::endl;;
    servoControl servoA(pinN); //initalize with given pin nr
    servoA.servoInit();
    servoControl servoB(pinN2); //initalize with given pin nr
    servoB.servoInit();

    for(int i = 0;i < 5;i++)
    {
        servoA.setDutyCycle(820000);
        servoB.setDutyCycle(820000);
        std::cout << "trying 82" << std::endl;
        std::cin.get();
        servoA.setDutyCycle(1900000);
        servoB.setDutyCycle(1900000);
        std::cout << "trying 19" << std::endl;
        std::cin.get();
    }
    std::cout << "end of test !"<< std::endl;
    return 0;
}

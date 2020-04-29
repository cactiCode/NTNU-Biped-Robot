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

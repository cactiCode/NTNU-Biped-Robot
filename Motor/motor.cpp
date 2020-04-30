#include <iostream>
#include <fstream>
#include <string>


class motor
{
    public:

    motor(int pinNumber)
    {
        m_dutyCycle = 1000000;
        setpwmPin (pinNumber);
        std::cout<< "Running with pin " << pinNumber << std::endl;
    }
    void setDutyCycle(int dutyC)
    {
        std::ofstream DCfile;
        DCfile.open("/sys/class/pwm/pwm-" + m_PinpathNumber + "/duty_cycle");
        DCfile << dutyC;
        DCfile.close();
    }
    void setPWMfreq(int frequency)
        {
            disablePWM();
            std::ofstream freqSet;
            freqSet.open("/sys/class/pwm/pwm-" + m_PinpathNumber + "/period");
            freqSet << frequency;
            freqSet.close();
            enablePWM();
        }

    void GPIOtoggle(int cape_nr,int pin_nr,int state)
    {   
        //intit addresses, states, and direction,
        setGPIOpin(cape_nr,pin_nr);
        setGPIOstate();
        //exportIOpin();
        setGPIOdirection("out");

        // Toggeling pin
        std::ofstream pinstream;
        pinstream.open("/sys/class/gpio/gpio"+ m_GPIONumber + "/value");
        pinstream << state;
        pinstream.close();
    }
    void toggleCurrentGPIOPin(int state)
    {
          // Toggeling pin
        std::ofstream pinstream;
        pinstream.open("/sys/class/gpio/gpio"+ m_GPIONumber + "/value");
        pinstream << state;
        pinstream.close();
    }
    void motorInit()
    {   
        setPWMstate();
        exportPWM();
        setPWMfreq(m_frequency);
        setDutyCycle(m_dutyCycle);
        enablePWM();  
    }
    private:

        int m_frequency = 20000000;
        int m_dutyCycle;
        int m_PIN;
        int m_pinbool;
        std::string m_Pinpath;
        std::string m_PinpathNumber;
        std::string m_pinChip;

        std::string m_GPIONumber;
        std::string m_IOpinpath;

        void setGPIOpin(int cape_nr,int IO_pin) //change to return 1 if good, 0 bad.
        {
            switch (cape_nr)
            {
                case 9:
                    switch (IO_pin)  //not GPIO but pins from beaglebone cape
                    {
                        case 23:
                        m_IOpinpath = "P9_23_pinmux";
                        m_GPIONumber = "49";
                        break;

                        case 25:
                        m_IOpinpath = "P9_25_pinmux";
                        m_GPIONumber = "117";
                        break;

                        case 27:
                        m_IOpinpath = "P9_27_pinmux";
                        m_GPIONumber = "115";
                        break;

                        default:
                        std::cout<<IO_pin << " is not an implemented pin"<< std::endl;
                        break;

                    }
                break;
                case 8:
                    switch (IO_pin)  //not GPIO but pins from beaglebone cape
                    {
                        case 34:
                        m_IOpinpath = "P8_34_pinmux";
                        m_GPIONumber = "81";
                        break;

                        case 36:
                        m_IOpinpath = "P8_36_pinmux";
                        m_GPIONumber = "80";
                        break;

                        case 38:
                        m_IOpinpath = "P8_38_pinmux";
                        m_GPIONumber = "79";
                        break;

                        default:
                        std::cout<<IO_pin << " is not an implemented pin"<< std::endl;
                        break;
                    }
                break;
                default:
                std::cout<<"pin : " <<cape_nr << " is an invalid cape_nr !" <<std::endl;
                break;
            }
        }
        void setGPIOstate()
        {
            std::ofstream IOstateStream;
            IOstateStream.open("sys/devices/platform/ocp/ocp:" + m_IOpinpath + "/state");
            IOstateStream << "default";
            IOstateStream.close();
        }
        void exportIOpin()
        {
            std::ofstream IOexportstream;
            IOexportstream.open("/sys/class/gpio/export");
            IOexportstream << m_GPIONumber;
            IOexportstream.close();
        }
        void setGPIOdirection(std::string direction)
        {
            std::ofstream IOdirStream;
            IOdirStream.open("/sys/class/gpio/gpio" + m_GPIONumber + "/direction");
            if (IOdirStream.fail())
            {
                exportIOpin();
                setGPIOdirection(direction);
                return;
            }
            IOdirStream << direction;
            IOdirStream.close();
        }


        void setpwmPin(int pin)
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
                case 14:
                    m_Pinpath = "P9_14_pinmux";
                    m_PinpathNumber = "4:0";
                    m_pinChip = "pwmchip4";
                    m_pinbool = 0;
                    break;
                case 16:
                    m_Pinpath = "P9_16_pinmux";
                    m_PinpathNumber = "4:1";
                    m_pinChip = "pwmchip4";
                    m_pinbool = 1;
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
        void disablePWM()
        {
            std::ofstream pwmEnable;
            pwmEnable.open("/sys/class/pwm/pwm-" + m_PinpathNumber + "/enable");
            pwmEnable << 0;
            pwmEnable.close();
        }

        void exportPWM ()
        {
            std::ofstream PWMexportStream;
            PWMexportStream.open("/sys/class/pwm/" + m_pinChip + "/export");
            PWMexportStream << m_pinbool;
            PWMexportStream.close();
        }
        

};
int main()
{
    int pinN = 29;          //pin_29 beaglebone cape set to pwm
    motor control(pinN);
    control.motorInit();

    
    control.setDutyCycle(1000000);  // 80%
    control.setPWMfreq(80000000);   //100Hz
    control.GPIOtoggle(9,23,1); // MOTOR OFF

    while(1)
    {
    control.GPIOtoggle(9,25,0);  //set GPIO pin 25 on cape 9 to 0
    control.GPIOtoggle(9,23,0);  //set GPIO pin 23 on cape 9 to 0   counterclockwise
    std::cin.get();
    control.GPIOtoggle(9,25,1); //set GPIO pin 25 on cape 9 to 1
    control.GPIOtoggle(9,23,1); //set GPIO pin 25 on cape 9 to 1    clockwise
    std::cin.get();
    }
    return 0;
}

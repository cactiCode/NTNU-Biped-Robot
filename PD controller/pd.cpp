
#include "pd.h"

//*********************
//Constructor function
//*********************

PD_Controller::PD_Controller(float kp, float kd)
{
  Initialise();
  KP=kp;
  KD=kd;
}

//Initialisation

PD_Controller::Initialise(){
  float error = 0.0;
  float previous_error_1 = 0.0;
  float previous_error_2 = 0.0;
  float previous_controlSignal=0.0;
}

//*************************
//Configuration functions
//*************************


void PD_Controller::Set_Parameters(float kp, float kd)
{
  float KP=kp;
  float KD=kd;
}

void PD_Controller::SetOutputRange(float min_Output, float max_Output)
{
  float MAX_OUTPUT=max_Output;
  float MIN_OUTPUT=min_Output;
}

//***************************
//Primary operating functions
//***************************

void PD_Controller::Set_Setpoint (int set_SetPoint)
{
  int Set_Setpoint=set_SetPoint;
}


float PD_Controller::getControllValue(float set_SetPoint, float sensor)
{
  float error = set_SetPoint - sensor;
  float controlSignal = previous_controlSignal+(KP*(error - previous_error_1))+(KD*(error-2*previous_error_1+previous_error_2));

 if (controlSignal>MAX_OUTPUT)
 {
   controlSignal=MAX_OUTPUT;
  }
  else if (controlSignal<MIN_OUTPUT)
  {
    controlSignal=MIN_OUTPUT;
  }
  return controlSignal;
}

void PD_Controller::Update()
{
  float previous_error_2=previous_error_1;
  float previous_error_1 = error;
  float previous_controlSignal=controlSignal;
}

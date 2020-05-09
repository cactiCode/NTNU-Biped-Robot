#include <float.h>
#include "PD_controller.h"

//*********************
//Constructor function
//*********************

PD_Controller::PD_Controller(float kp, float kd);{
  Initialise();
  Kp=kp;
  Kd=kd;
}

//Initialisation

PD_Controller::Initialise(){
  flat KP = 0.0f;
  float KD = 0.0f;
  float error = 0.0f;
  float previous_error_1 = 0.0f;
  float previous_error_2 = 0.0f;
  float MAX_OUTPUT = 0.0f; //max voltage from the controller
  float MIN_OUTPUT = 0.0f; // min voltage from the controller

  firstRun=true;

}

//*************************
//Configuration functions
//*************************


void PD_Controller::Set_Parameters(float kp, float kd){
  float KP=kp;
  float KD=kd;

}


void PD_Controller::SetOutputRange(float min_Output, float max_Output);{
  float MAX_OUTPUT=max_Output;
  float MIN_OUTPUT=min_Output;

}

//***************************
//Primary operating functions
//***************************

void PD_Controller::Set_Setpoint (float set_SetPoint){
  Set_Setpoint=set_SetPoint;

}


void PD_Controller::getControllValue(float set_SetPoint,float encoder){
  float error=setPoint - encoder;

  float controlSignal=previous_controlSignal+(Kp(error - previous_error_1))+(Kd*(error-2*previous_error_1+previous_error_2));


  if (controlSignal>MAX_OUTPUT) {
    controlSignal=max_Output;
  }   else if (controlSignal<MIN_OUTPUT){
   controlSignal=min_Output;
  }

}

void PD_Controller::Update() {
  float previous_error_2=previous_error_1;
  float previous_error_1 = error;
  float previous_controlSignal=controlSignal;
}

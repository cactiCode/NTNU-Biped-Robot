#ifndef pd.h
#define pd.h

class PD_Controller
{
  private:
  float KP,KD;
  float controlSignal;
  float Set_Setpoint;
  float previous_controlSignal;
  float error;
  float previous_error_1;
  float previous_error_2;
  float MAX_OUTPUT;
  float MIN_OUTPUT;


  public:
  PD_Controller(float, float);
  float getControllValue(float);
  void Set_Parameters(float, float);
  void SetOutputRange(float, float);
  void Set_Setpoint(float);
  void Update();
  void Initialise();
};

#endif

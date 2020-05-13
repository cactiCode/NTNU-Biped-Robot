#include "ros/ros.h"
#include "std_msgs/int32.h"


//PD controller class:
class PD_Controller
{

  private:
    float KP,KD;
    float Set_Setpoint;
    float controlSignal;
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
    void setPD(float, float);
    int32 output();

  PD_Controller(float kp, float kd)
  {
    Initialise();
    KP=kp;
    KD=kd;
  }

  //Initialisation

  void Initialise()
  {
    float error = 0.0f;
    float previous_error_1 = 0.0f;
    float previous_error_2 = 0.0f;
  }
  //*************************
  //Configuration functions
  //*************************
  void Set_Parameters(float kp, float kd){
    float KP=kp;
    float KD=kd;

  }

  void SetOutputRange(float min_Output, float max_Output);{
    float MAX_OUTPUT=max_Output;
    float MIN_OUTPUT=min_Output;
  }
  //***************************
  //Primary operating functions
  //***************************
  void Set_Setpoint (float setpoint){
    this -> Set_Setpoint=setpoint;
  }

   string getControllValue(int32 Set_Setpoint,int32 encoder)
   {
     string error = Set_Setpoint - encoderPosition;
     string controlSignal = previous_controlSignal+(Kp*(error - previous_error_1))+(Kd*(error-2*previous_error_1+previous_error_2));

     if (controlSignal>MAX_OUTPUT)
      {
      controlSignal=max_Output;
      }   else if (controlSignal<MIN_OUTPUT)
      {
        controlSignal=min_Output;
      }
      return controlSignal;
    }

  //ERROR AND CONTROLSIGNAL UPDATE
  void Update() {
    float previous_error_2 = previous_error_1;
    float previous_error_1 = error;
    float previous_controlSignal = controlSignal;
  }
};


//********************************************
//Robot operating system
//********************************************
// ROS subscriber functions
void encoderCall_back(const std_msgs::Int32::ConstPtr& msg)
{
  ROS_INFO("controller_recieves_encoder_data: %d", msg->data);
  int encoderPosition = msg -> data.c_str();
}

void setpointCall_back(const std_msgs::int32::ConstPtr& msg2)
{
  ROS_INFO("controller_recieves_setpoint_data. %d", msg2->data2);
  int setpointValue=msg2 -> data2.c_str();
}

int main(int argc, char** argv)
{
  PD_Controller myPD (10,1); //Creating an object
  myPD.SetOutputRange(3,0); //Setting max and min signal values
  myPD.Set_Setpoint(setpointValue); //Setting setpoint value via ROS topic "setpoinValue"
  //******************************************************************************

  //Ros code
  ros::init(argc, argv, "controller_node");
  ROS_INFO("Starting PD controller node");
  ros::NodeHandle controller_node;
  while (ros::ok() && ros::Time(0) == ros::Time::now())
  {
    ROS_INFO("controller_node is initiated");
    sleep(1);
  }
  while (ros::ok())
  {

    // Subscribers and publishers
    ros::Subscriber setpoint_sub = controller_node.subscribe<std_msgs::int32<("setpoint",1000,setpointCall_back);
    ros::Subscriber encoder_sub = controller_node.subscribe<std_msgs::int32<("setpoint",1000,encoderCall_back);
    ros::Publisher controller_pub = controller_node.advertise<std_msgs::int32>("controller", 1000);
    ros::Rate loop_rate(10);
  }


  while (ros::ok())
  {

    myPD.PD_Controller(10,1);
    myPD.getControllValue(setpoinValue, encoderPosition);

    std_msgs::String msg;
    std::stringstream ss;
    <<myPD.getControllValue()<< std::endl;
    msg.data = myPD.getControllValue


    controller_pub.publish(msg);  // publish
    ros::spin();
    loop_rate.sleep();

    ++count;
  }
  return 0;
}

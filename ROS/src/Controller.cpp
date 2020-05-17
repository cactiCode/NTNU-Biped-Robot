#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"



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

  PD_Controller(float kp, float kd)
  {
    Initialise();
    KP=kp;
    KD=kd;
  }

  //Initialisation

  void Initialise()
  {
    float error = 0.0;
    float previous_error_1 = 0.0;
    float previous_error_2 = 0.0;
  }
  //*************************
  //Configuration functions
  //*************************
  void Set_Parameters(float kp, float kd)
  {
    float KP=kp;
    float KD=kd;

  }

  void SetOutputRange(float min_Output, float max_Output)
  {
    float MAX_OUTPUT=max_Output;
    float MIN_OUTPUT=min_Output;
  }
  //***************************
  //Primary operating functions
  //***************************
  void Set_Setpoint (float setpoint)
  {
    float Set_Setpoint = setpoint;
  }

   string getControllValue(int setpointValue,int encoderPosition)
   {
     string error = setpointValue - encoderPosition;
     string controlSignal = previous_controlSignal+(KP*(error - previous_error_1))+(KD*(error-2*previous_error_1+previous_error_2));

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
  void Update()
   {
    float previous_error_2 = previous_error_1;
    float previous_error_1 = error;
    float previous_controlSignal = controlSignal;
  }
};


//********************************************
//Robot operating system
//********************************************
// ROS callback function
void encoderCall_back(const std_msgs::Int32::ConstPtr& encoderPosition)
{
  std_msgs::Int32 encoder //Topic name
  encoder=encoderPosition->data;
  ROS_INFO("controller_recieves_encoder_data: %d", encoder_msg.data)
}

void setpointCall_back(const std_msgs::Int32::ConstPtr& setpointValue)
{
  std_msgs::Int32 setpoint
  setpoint=setpointValue->data;
  ROS_INFO("controller_recieves_setpoint_data: %d", setpoint.data)
}

int main(int argc, char** argv)
{
  PD_Controller myPD (10,1); //Creating an object with KP and KD values
  myPD.SetOutputRange(3,0); //Setting max and min signal values
  myPD.Set_Setpoint(1000);//Setting setpoint
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

  // Subscribers and publishers

    ros::Subscriber encoder_sub = controller_node.subscribe<std_msgs::Int32>("chatter",100,encoderCall_back);
    ros::Subscribe setpoint_sub = controller_node.subscribe<std_msgs::Int32>("setpoint",100,setpointCall_back);
    ros::Publisher controller_pub = controller_node.advertise<std_msgs::Int32>("controller", 100);
    ros::Rate loop_rate(100);

  int count = 0;
  while (ros::ok())
  {
    myPD.getControllValue(setpoint, encoder);
    myPD.Update();


    std_msgs::Int32 message;
    message.data = myPD.getControllValue;


    controller_pub.publish(message.data);  // publish
    ros::spinOnce();
    loop_rate.sleep();

    ++count;
  }
  return 0;
}

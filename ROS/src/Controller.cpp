#include "ros/ros.h"
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
  void Set_Setpoint (int32 setpoint)
  {
    int32 Set_Setpoint = setpoint;
  }

   string getControllValue(int32 setpointValue,int32 encoderPosition)
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

class Publisher_Subscriber
{
  public:
    Publisher_Subscriber()
    {
      publisherObject = n.advertise<std_msgs::Int32>("controller", 1000);
      subscriberObject = n.subscribe<std_msgs::Int32>("chatter", 1000, &Publisher_Subscriber::subscriberCallback, this);
    }
    void subscriberCallBack (const std_msgs::Int32::Subscribe::ConstPtr& encoderPosition);

  private:
    ros::Subscriber subscriberObject;
    ros::Publisher publisherObject;
    ros::NodeHandle n;

  void subscriberCallback(const std_msgs::Int32::ConstPtr& encoderPosition)
  {
    std_msgs::Int32 encoder //Topic name
    encoder_msg.data=encoderPosition->data;
    ROS_INFO("controller_recieves_encoder_data: %d", encoder_msg.data);
  }
};
int main(int argc, char** argv)
{
  PD_Controller myPD (10,1); //Creating an object with KP and KD values
  myPD.SetOutputRange(3,0); //Setting max and min signal values
  myPD.Set_Setpoint(1000);//Setting setpoint
  //******************************************************************************
  //Ros code
  ros::init(argc, argv, "controller_node");
  ROS_INFO("Starting PD controller node");

  while(ros::ok()){
    myPD.getControllValue(setpoinValue, encoderPosition);
    myPD.Update();

    Publisher_Subscriber SUB_PUB_Object; //Creating object for publisher subscriber class
    std_msgs::Int32 message;
    message.data = myPD.getControllValue;

    ros::Rate loop_rate(100);
    controller_pub.publish(message);  // publish
    ros::spinOnce();
    loop_rate.sleep();

    ++count;
  }
  return 0;
}

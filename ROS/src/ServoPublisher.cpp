#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
    //ROS code
    ros::init(argc,argv,"ServoPublisher");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Rate loop_rate(3);

    int count = 0;

    while (ros::ok())
    {
        std_msgs::String msg;
        //std::stringstream ss;
        if ((count % 3) == 0) 
        { 
            int count2 = 0;
            if ((count2 % 3) == 0) 
            {
                msg.data = "1";
            }
                //ss << "1" << std::endl;   
           
        }   
         else
            msg.data = "0";
                //ss << "0" << std::endl;
        //ss << "[ Count ] : " << count << std::endl; 
        //msg.data = ss.str();
        //ROS_INFO("%s", msg.data.c_str());


        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
}

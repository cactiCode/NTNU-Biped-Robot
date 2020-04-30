#include "ros/ros.h"
#include "std_msgs/String.h"

#include<iostream>

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("hear : [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "IMUsubscriber");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    ros::spin();
    return 0;
}

#pragma once

#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"
#include "rosbag/bag.h"

class DevJoyController {

private:

    ros::NodeHandle nh;

    ros::Subscriber sub;    // 조이스틱이 보내는 publish 를 받아서
    ros::Publisher pub;     // /cmd_vel로 publish 함

    double linear;
    double angular;

    double max_linear;
    double max_angular;

    /*
    The rosbag C++ API works on the premise of creating "views" of one or more bags using "queries".
    A Query is an abstract class which defines a function that filters whether or not the messages from a connection are to be included.
    This function has access to topic_name, datatype, md5sum, message definition as well as the connection header.
    Additionally, each Query can specify a start and end time for the range of times it includes.
    */
    rosbag::Bag cmd_vel_bag;

public:

    // constructor & destructor
    DevJoyController(double max_linear,double max_angular);
    ~DevJoyController();

    // callback function
    void joyCallback(const sensor_msgs::Joy &msg);

    // setter
    void setLinear(double linear) { this->linear = linear; }
    void setAngular(double angular) { this->angular = angular; }

    // getter
    double getLinear() { return this->linear; }
    double getAngular() { return this-> angular; }
    double getMaxLinear() { return this->max_linear; }
    double getMaxAngular() { return this->max_angular; }

};
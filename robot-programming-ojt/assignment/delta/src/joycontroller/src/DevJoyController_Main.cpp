#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"
#include "joycontroller/DevJoyController.h"

#include "rosbag/bag.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"

int main(int argc, char **argv) {

    ros::init(argc, argv, "dev");

    double max_linear, max_angular;
    max_linear = 0;
    max_angular = 0;

    ros::NodeHandle nh_param("~");
    nh_param.getParam("max_linear", max_linear);
    nh_param.getParam("max_angular", max_angular);

    DevJoyController devJoyController(max_linear, max_angular);

    while (ros::ok()) {
        ros::spin();
    }

    delete &devJoyController;

    return 0;
}
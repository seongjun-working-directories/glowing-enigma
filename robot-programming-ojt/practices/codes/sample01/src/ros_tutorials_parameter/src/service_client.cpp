#include "ros/ros.h"
#include "ros_tutorials_parameter/SrvTutorial.h"
#include <cstdlib>

int main(int argc, char **argv) {
  ros::init(argc, argv, "parameter_server");

  if (argc != 3) {
    ROS_INFO(">>> cmd: rosrun ros_tutorials_service service_client arg0 arg1");
    ROS_INFO(">>> arg0: double number, arg1: double number");
    return 1;
  }

  ros::NodeHandle nh;

  ros::ServiceClient ros_tutorials_parameter_client = nh.serviceClient<ros_tutorials_parameter::SrvTutorial>("ros_tutorial_parameter");

  ros_tutorials_parameter::SrvTutorial srv;

  srv.request.a = atoll(argv[1]);
  srv.request.b = atoll(argv[2]);

  if (ros_tutorials_parameter_client.call(srv)) {
    ROS_INFO(
      "send srv, srv.Request.a and srv.Request.b are %ld, %ld",
      (long int)srv.request.a,
      (long int)srv.request.b
    );
    ROS_INFO(
      "receive srv, srv.Response.result is %ld",
      (long int)srv.response.result
    );
  }
  else {
    ROS_ERROR("FAILED TO CALL SERVICE ROS_PARAMETER_SRV");
    return 1;
  }

  return 0;
}
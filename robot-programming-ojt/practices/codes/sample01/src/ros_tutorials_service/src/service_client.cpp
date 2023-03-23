// ROS Default Header
#include "ros/ros.h"
// SrvTutorial Service File Header
#include "ros_tutorials_service/SrvTutorial.h"
// Library to use 'atoll' function
#include <cstdlib>

// Node Main Function
int main(int argc, char **argv) {
  // Initialization of Node Name
  ros::init(argc, argv, "service_server");

  // Handling Possible Input Value Error
  if (argc != 3) {
    ROS_INFO(">>> cmd: rosrun ros_tutorials_service service_client arg0 arg1");
    ROS_INFO(">>> arg0: double number, arg1: double number");
    return 1;
  }

  // Declaration of Node Handle
  ros::NodeHandle nh;

  // Declaration of service server 'ros_tutorials_service_client'
  ros::ServiceClient ros_tutorials_service_client = nh.serviceClient<ros_tutorials_service::SrvTutorial>("ros_tutorial_srv");
  
  // Declaration of 'srv' service that uses the 'SrvTutorial' service file
  ros_tutorials_service::SrvTutorial srv;

  // Parameters entered when the node is executed as a service request value
  // Store in 'a' and 'b'
  srv.request.a = atoll(argv[1]);
  srv.request.b = atoll(argv[2]);

  // Request the service : ```ros_tutorials_service_client.call(srv)```
  // Only if the request is accepted --> Display the response value
  if (ros_tutorials_service_client.call(srv)) {
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
    ROS_ERROR("FAILED TO CALL SERVICE ROS_SERVICE_SRV");
    return 1;
  }

  return 0;
}
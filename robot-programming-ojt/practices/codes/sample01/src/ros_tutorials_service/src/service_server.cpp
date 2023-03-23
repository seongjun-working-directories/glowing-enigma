// ROS Default Header
#include "ros/ros.h"
// SrvTutorial Service File Header
#include "ros_tutorials_service/SrvTutorial.h"

// Performed when a service request exists
bool calculation(
  ros_tutorials_service::SrvTutorial::Request &req, // service request
  ros_tutorials_service::SrvTutorial::Response &res // service response
) {
  // The service 'ros_tutorial_srv' calls 'calculation' function upon the service request
  res.result = req.a + req.b;

  // request 및 response에 대한 정보를 출력
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: %ld", (long int)res.result);

  return true;
}

// Node Main Function
int main(int argc, char **argv) {
  // Initialization of Node Name
  ros::init(argc, argv, "service_client");

  // Declaration of Node Handle
  ros::NodeHandle nh;

  // Declaration of service server 'ros_tutorials_service_server'
  ros::ServiceServer ros_tutorials_service_server = nh.advertiseService("ros_tutorial_srv", calculation);

  ROS_INFO("ready srv server!");

  // Wait for the service request
  ros::spin();

  return 0;
}
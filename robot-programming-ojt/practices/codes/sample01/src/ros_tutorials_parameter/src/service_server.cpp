#include "ros/ros.h"
#include "ros_tutorials_parameter/SrvTutorial.h"

#define ADDITION 1
#define SUBSTRACTION 2
#define MULTIPLICATION 3
#define DIVISION 4

int g_operator = ADDITION;

bool calculation(
  ros_tutorials_parameter::SrvTutorial::Request &req,
  ros_tutorials_parameter::SrvTutorial::Response &res
) {
  switch(g_operator) {
    case ADDITION:
      res.result = req.a + req.b;
      break;
    case SUBSTRACTION:
      res.result = req.a - req.b;
      break;
    case MULTIPLICATION:
      res.result = req.a * req.b;
      break;
    case DIVISION:
      res.result = req.a * req.b;
      break;
    default:
      res.result = req.a + req.b;
  }

  ROS_INFO("request: a=%ld, b=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("response: result=%ld", (long int)res.result);

  return true;
}

int main(int argc, char **argv) {
  ros:: init(argc, argv, "parameter_client");

  ros::NodeHandle nh;

  // Reset Parameter Settings : 파라미터 초기 설정
  nh.setParam("calculation_method", ADDITION);

  ros::ServiceServer ros_tutorials_service_server = nh.advertiseService("ros_tutorial_parameter", calculation);

  ROS_INFO("ready parameter server!");

  // 10 hz == 0.1 sec
  ros::Rate r(10);

  while (ros::ok()) {
    // Select the operator according to the value received from the parameter
    // 연산자를 매개변수로부터 받은 값으로 변경
    nh.getParam("calculation_method", g_operator);

    ros::spinOnce();
    r.sleep();
  }

  return 0;
}
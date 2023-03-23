// #pragma once
#ifndef TURTLE_LAWNMOWER_HPP
#define TURTLE_LAWNMOWER_HPP

#include "ros/ros.h"
#include "turtlesim/Pose.h"

#include <std_srvs/Empty.h>
#include "std_msgs/Bool.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/Kill.h"

#include <string>

class TurtleLawnmower {

  ros::NodeHandle nh;
  ros::Subscriber sub;
  ros::Publisher pub;

  ros::Subscriber msg_sub;
  ros::Publisher msg_pub;

  ros::ServiceClient reset;
  turtlesim::Pose turtlesim_pose;

  ros::ServiceClient spawn;
  ros::ServiceClient kill_spawn;

  turtlesim::Spawn srv;
  turtlesim::Kill kill;

  bool start;

  public:
  TurtleLawnmower();
  ~TurtleLawnmower();

  void turtleCallback(const turtlesim::Pose::ConstPtr& msg);

  void boolCallback(const std_msgs::BoolConstPtr &msg);

  void move(double speed, double distance, bool isForward);

  void rotate(double angular_speed, double angle_radian, bool direction);

  void circle(bool bigSize, bool halfCircle, bool direction);

  void callReset(std_srvs::Empty empty);

  void callSpawn();

  void callKill();

  bool getStart();

};

#endif
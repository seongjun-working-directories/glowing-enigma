#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "string.h"

class Player {
private:
  double linear_x, angular_z;
  double max_linear_x, max_angular_z;

  std::string record;
  std::string filename;

  ros::NodeHandle nh;
  ros::Subscriber joysub;
  ros::Publisher servinggopub;

public:
  Player(double max_linear,double max_angular, std::string filename);
  ~Player();
  void playerCallback(const sensor_msgs::Joy &msg);
  void cmdVelRecorder(double linear_x, double angular_z);
  void recordSaver();
};

#endif
#ifndef RECORDER_HPP
#define RECORDER_HPP

#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "string.h"

class Recorder {
private:
  double linear_x, angular_z;
  double max_linear_x, max_angular_z;

  std::string record;
  std::string filename;

  ros::NodeHandle nh;
  ros::Subscriber joysub;
  ros::Publisher turtlepub;

public:
  Recorder(double max_linear,double max_angular, std::string filename);
  ~Recorder();
  void recorderCallback(const sensor_msgs::Joy &msg);
  void cmdVelRecorder(double linear_x, double angular_z);
  void recordSaver();
};

#endif
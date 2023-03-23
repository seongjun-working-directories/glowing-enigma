#include "turtle_to_servinggo/Recorder.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include "string.h"

#include <iostream>
#include <fstream>

Recorder::Recorder(double max_linear, double max_angular, std::string filename) {
  // 최대 속력을 저장함
  this->max_linear_x = max_linear;
  this->max_angular_z = max_angular;

  // 조이스틱 값을 Subscribe 하여 txt 파일에 일정 양식에 맞게 저장함
  joysub = nh.subscribe("/joy", 1, &Recorder::recorderCallback, this);
  // 들어온 조이스틱 값을 시각화하기 위해 turtlesim을 활용함
  turtlepub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);

  // 저장할 내용을 담은 변수
  this->record = "";
  // 저장할 파일명을 담은 변수
  this->filename = filename;
};

Recorder::~Recorder() {

};

void Recorder::recorderCallback(const sensor_msgs::Joy &msg) {
  geometry_msgs::Twist getCmdVel;

  getCmdVel.linear.x = msg.axes[1] * (this->max_linear_x);
  getCmdVel.angular.z = msg.axes[0] * (this->max_angular_z);

  this->linear_x = msg.axes[1] * (this->max_linear_x);
  this->angular_z = msg.axes[0] * (this->max_angular_z);

  // linear_x와 angular_z를 인자로 념겨 txt 파일로 저장되도록 함
  Recorder::cmdVelRecorder(this->linear_x, this->angular_z);

  // turtle1/cmd_Vel로 getCmdVel을 쏴서 turtlesim_node로 볼 수 있도록 함
  this->turtlepub.publish(getCmdVel);
};

void Recorder::cmdVelRecorder(double linear_x, double angular_z) {
  std::string str_linear_x = std::to_string(linear_x);
  std::string str_angular_z = std::to_string(angular_z);
  std::string record_line = str_linear_x + " " + str_angular_z + "\n";
  this->record = record.append(record_line);
};

void Recorder::recordSaver() {
  std::ofstream output(this->filename);
  output << record;
  // output.close();  // 스코프를 벗어나는 순간 알아서 close() 진행됨
  std::cout << "Successfully Saved!" << std::endl;
};
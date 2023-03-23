#include "turtle_to_servinggo/Player.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include "string.h"

#include <iostream>
#include <fstream>

Player::Player(double max_linear, double max_angular, std::string filename) {
  this->max_linear_x = max_linear;
  this->max_angular_z = max_angular;

  joysub = nh.subscribe("/joy", 1, &Player::playerCallback, this);
  servinggopub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

  this->record = "";
  this->filename = filename;
};

Player::~Player() {

};

void Player::playerCallback(const sensor_msgs::Joy &msg) {
  geometry_msgs::Twist getCmdVel;

  getCmdVel.linear.x = msg.axes[1] * (this->max_linear_x);
  getCmdVel.angular.z = msg.axes[0] * (this->max_angular_z);

  this->linear_x = msg.axes[1] * (this->max_linear_x);
  this->angular_z = msg.axes[0] * (this->max_angular_z);

  Player::cmdVelRecorder(this->linear_x, this->angular_z);
  this->servinggopub.publish(getCmdVel);
};

void Player::cmdVelRecorder(double linear_x, double angular_z) {
  std::string str_linear_x = std::to_string(linear_x);
  std::string str_angular_z = std::to_string(angular_z);
  std::string record_line = str_linear_x + " " + str_angular_z + "\n";
  this->record = record.append(record_line);
};

void Player::recordSaver() {
  std::ofstream output(this->filename);
  output << record;
  std::cout << "Successfully Saved!" << std::endl;
};
// #pragma once
#ifndef TURTLE_LAWNMOWER_FUNCTION_CPP
#define TURTLE_LAWNMOWER_FUNCTION_CPP

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <turtle_lawnmower/TurtleLawnmower.hpp>
#include <string>

// constructor initializes the subscriber and telling it to call turtleCallback
// which is method in the TurtleLawnmower class
TurtleLawnmower::TurtleLawnmower() : start(false)
{
  // turtle에 대한 pub, sub 초기화
  sub = nh.subscribe("turtle1/pose", 1, &TurtleLawnmower::turtleCallback, this);
  pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);

  // trigger message에 대한 pub, sub 초기화
  msg_sub = nh.subscribe("/driver_start", 1, &TurtleLawnmower::boolCallback, this);
  msg_pub = nh.advertise<std_msgs::Bool>("/driver", 1);

  // spawn, kill, reset 초기화
  spawn = nh.serviceClient<turtlesim::Spawn>("spawn");
  kill_spawn = nh.serviceClient<turtlesim::Kill>("kill");
  reset = nh.serviceClient<std_srvs::Empty>("reset");
}

TurtleLawnmower::~TurtleLawnmower() {}

// 일정 주기마다 publish된 turtle1/pose의 좌표를 출력
void TurtleLawnmower::turtleCallback(const turtlesim::Pose::ConstPtr &msg) {
  ROS_INFO("Turtle Lawnmower@[%f, %f, %f]", msg->x, msg->y, msg->theta);

  geometry_msgs::Twist turtle_cmd_vel;
  turtlesim_pose.x = msg->x;
  turtlesim_pose.y = msg->y;
  turtlesim_pose.theta = msg->theta;
}

// bool 변수를 true로 바꿔서 메인 로직이 시작될 수 있도록 함
void TurtleLawnmower::boolCallback(const std_msgs::BoolConstPtr &msg) {
  ROS_INFO("START by command 'rostopic pub /driver_start std_msgs/Bool \"true\"");
  // [TEST]ROS_INFO("%d", start);
  start = msg->data;
  // [TEST] ROS_INFO("%d", start);
}

// 전진 및 후진 : speed로 속도를 결정 || distance로 거리를 결정 || isForward를 통해 앞뒤를 결정
void TurtleLawnmower::move(double speed, double distance, bool isForward) {
  geometry_msgs::Twist turtle_cmd_vel;

  if (isForward) { turtle_cmd_vel.linear.x = fabs(speed); }
  else { turtle_cmd_vel.linear.x = -fabs(speed); }

  // 속도와 시간을 설정하여 거리를 조절하는 로직
  double current_distance = 0.0;
  double t0 = ros::Time::now().toSec();
  ros::Rate rate(250);

  while (current_distance <= distance) {
    pub.publish(turtle_cmd_vel);
    rate.sleep();
    double t1 =  ros::Time::now().toSec();
    current_distance = speed * (t1 - t0);
    ros::spinOnce();
  }

  turtle_cmd_vel.linear.x = 0.0;
  pub.publish(turtle_cmd_vel);
}

// 우회전 및 좌회전
void TurtleLawnmower::rotate(double angular_speed, double angle_radian, bool direction) {
  geometry_msgs::Twist turtle_cmd_vel;

  if (direction) { turtle_cmd_vel.angular.z = abs(angular_speed); }
  else { turtle_cmd_vel.angular.z = -abs(angular_speed); }

  double current_angle = 0.0;
  double t0 = ros::Time::now().toSec();
  ros::Rate rate(300);

  while (current_angle <= angle_radian) {
    pub.publish(turtle_cmd_vel);
    rate.sleep();
    double t1 = ros::Time::now().toSec();

    current_angle = angular_speed * (t1 - t0);
    ros::spinOnce();
  }

  turtle_cmd_vel.angular.z = 0.0;
  pub.publish(turtle_cmd_vel);
}

void TurtleLawnmower::circle(bool bigSize, bool halfCircle, bool direction) {
  geometry_msgs::Twist turtle_cmd_vel;

  ROS_INFO("Circle Drawing Started...");

  double durationTime;

  if (bigSize) {
    if (!halfCircle) { durationTime = 2.0 * M_PI; }
    else { durationTime = M_PI; }
  }
  else {
    if (!halfCircle) { durationTime = M_PI; }
    else { durationTime = M_PI / 2.0; }
  }

  ros::Time now = ros::Time::now();

  while(ros::Time::now() <= now + ros::Duration(durationTime)) {
    turtle_cmd_vel.linear.x = 2.0;  // Front and rear (+ -) m/s
    turtle_cmd_vel.linear.y = 0.0;  // Left and right (+ -) m/s
    turtle_cmd_vel.linear.z = 0.0;

    turtle_cmd_vel.angular.x = 0;
    turtle_cmd_vel.angular.y = 0;

    // Rotation speed of robot, + left turn, - right turn, unit: rad/
    // 숫자는 원 크기, +와 -는 원을 그리는 방향
    if (direction)  {
      if (bigSize) { turtle_cmd_vel.angular.z = 1; }
      else { turtle_cmd_vel.angular.z = 2; }
    }
    else {
      if (bigSize) { turtle_cmd_vel.angular.z = -1; }
      else { turtle_cmd_vel.angular.z = -2; }
    }

    pub.publish(turtle_cmd_vel);

    ros::spinOnce();
  }

  turtle_cmd_vel.angular.z = 0.0;
  turtle_cmd_vel.linear.x = 0.0;
  pub.publish(turtle_cmd_vel);
}

void TurtleLawnmower::callReset(std_srvs::Empty empty) {
  reset.call(empty);
}

void TurtleLawnmower::callSpawn() {
  srv.request.x = 5.928058;
  srv.request.y = 5.446051;
  srv.request.name = "turtle2";

  spawn.call(srv);

  sub = nh.subscribe(
    "turtle2/pose",
    1,
    &TurtleLawnmower::turtleCallback,
    this
  );

  pub = nh.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 1);
}

void TurtleLawnmower::callKill() {
  kill.request.name = "turtle1";
  kill_spawn.call(kill);
}

bool TurtleLawnmower::getStart() {
  return start;
}

#endif
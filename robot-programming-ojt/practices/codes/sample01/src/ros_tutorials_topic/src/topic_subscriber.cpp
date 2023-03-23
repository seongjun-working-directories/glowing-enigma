// 빌드 후 자동 생성됨
#include "ros/ros.h"  // ROS 기본 헤더파일
#include "ros_tutorials_topic/MsgTutorial.h" // MsgTutorial 메시지 파일 헤더

// 메시지 콜백 함수
// 아래에 설정한 ros_tutorial_msgs라는 이름의 토픽 메시지를 수신했을 때 동작
// 입력 메시지로는 ros_tutorials_topic 패키지의 MsgTutorials 메시지를 받도록 되어 있음

// ros_tutorials_topic 안의 MsgTutorial의 ConstPtr 타입의 변수를 전달 받음
void msgCallback(const ros_tutorials_topic::MsgTutorial::ConstPtr& msg) {
  ROS_INFO("receive msg=%d", msg->stamp.sec);
  ROS_INFO("receive msg=%d", msg->stamp.nsec);
  ROS_INFO("receive msg=%d", msg->data);
}

// 노드 메인 함수
int main(int argc, char **argv) {
  // 노드명 초기화
  ros::init(argc, argv, "topic_subscriber");

  // ROS 시스템과의 통신을 위한 노드 핸들 선언
  ros::NodeHandle nh;

  // subscriber 선언
  // ros_tutorials_topic 패키지의 MsgTutorial 메시지 파일을 이용한 subscriber인 ros_tutorial_sub를 작성
  // topic의 이름은 "ros_tutorial_msg"이며, subscriber queue 사이즈를 100개로 설정
  ros::Subscriber ros_tutorial_sub = nh.subscribe("ros_tutorial_msg", 100, msgCallback);

  // 콜백함수 호출을 위한 함수
  // 메시지 수신을 기다리며, 수신되었을 경우 콜백함수를 실행함
  ros::spin();

  return 0;
}
// 빌드 후 자동 생성됨
#include "ros/ros.h"  // ROS 기본 헤더파일
#include "ros_tutorials_topic/MsgTutorial.h" // MsgTutorial 메시지 파일 헤더

// 노드 메인 함수
int main(int argc, char **argv) {
  // 노드명 초기화
  ros::init(argc, argv, "topic_publisher");

  // ROS 시스템과의 통신을 위한 노드 핸들 선언
  ros::NodeHandle nh;

  // publisher 선언
  // ros_tutorials_topic 패키지의 MsgTutorial 메시지 파일을 이용한 publisher인 ros_tutorial_pub을 작성
  // topic의 이름은 "ros_tutorial_msg"이며, publisher queue 사이즈를 100개로 설정
  ros::Publisher ros_tutorial_pub = nh.advertise<ros_tutorials_topic::MsgTutorial>(
    "ros_tutorial_msg", 100
  );

  // 루프 주기를 10Hz(0.1초 간격)로 설정
  ros::Rate loop_rate(10);

  // MsgTutorial 메시지 파일 형식으로 msg라는 메시지를 선언
  ros_tutorials_topic::MsgTutorial msg;

  // 메시지에 사용될 변수 선언
  int count = 0;

  while(ros::ok()) {
    // 현재시간을 stamp 메시지에 담음
    msg.stamp = ros::Time::now();
    // count 변수값을 data 메시지에 담음
    msg.data = count;

    ROS_INFO("send msg=%d", msg.stamp.sec); // stamp.sec 메시지를 표시
    ROS_INFO("send msg=%d", msg.stamp.nsec);  // stamp.nsec 메시지를 표시
    ROS_INFO("send msg=%d", msg.data);  // data 메시지를 표시

    ros_tutorial_pub.publish(msg);  // 메시지 발행

    loop_rate.sleep();  // 앞서 설정한 루프 주기에 따라 슬립

    ++count;  // count 변수를 1씩 증가시킴
  }

  return 0;
}
#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"

#include "sensor_msgs/PointCloud2.h"

#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "pcl2_controller_function.cpp"

// https://limhyungtae.github.io/2021-09-09-ROS-Point-Cloud-Library-(PCL)-0.-Tutorial-%EB%B0%8F-%EA%B8%B0%EB%B3%B8-%EC%82%AC%EC%9A%A9%EB%B2%95/
// https://wiki.ros.org/pcl_ros#Subscribing_to_point_clouds
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

// PointCloud2를 읽어오는 콜백 함수
void pcl2Callback(const sensor_msgs::PointCloud2 msg)
{
  PointCloud processed_msg = sensor2pcl(msg);

  printf("Cloud: width = %d, height = %d\n", processed_msg.width, processed_msg.height);
  BOOST_FOREACH(const pcl::PointXYZ& pt, processed_msg.points)
  {
    // 확인: processed_msg.points.size()는 256000(즉, 640 * 400px)임
    // 확인: 따라서 BOOST_FOREACH는 ROS_INFO를 256000번 출력한 후 종료됨
    ROS_INFO("\tAbout Point :: (%f, %f, %f)\n", pt.x, pt.y, pt.z);
  }
  
  // 초당 촬영면(640*400px)에 대한 정보는 rviz의 /camera/depth/points >> depth cloud X, image O 에서 확인 가능

  // 3. 각 값의 단위(mm, cm, m, inch, etc.)에 대한 정보가 필요함

  // 4. /camera/depth/points와 /camera/depth_registered/points의 차이점을 구분해야 함
  // 5. /camera/depth_registered/points가 들어오지 않는 이유를 알아야 함

  // 6. Opencv4로 해당 depth 값을 다룰 수 있어야 함 -> 깊이 값만 색깔로 변경하면 됨
}

int main(int argc, char** argv) {

  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>("/camera/depth/points", 1, pcl2Callback);
  ros::spin();

  return 0;
}
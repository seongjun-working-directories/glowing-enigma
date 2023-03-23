#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

// 해당 include는 오로지 LaserScan만을 위한 것이므로,
// LaserScan에 대한 형변환이 필요없는 상황에서 주석처리해도 무관함
#include "laser_geometry/laser_geometry.h"

// RGB-D의 Depth를 편리하게 쓰기 위한 형변환
// sensor_msgs::PointCloud2 -> pcl::PointCloud
pcl::PointCloud<pcl::PointXYZ> sensor2pcl(sensor_msgs::PointCloud2 raw_msg)
{
  pcl::PointCloud<pcl::PointXYZ> processed_msg;
  pcl::fromROSMsg(raw_msg, processed_msg);
  return processed_msg;
}

// 형변환하여 사용한 Depth 값을 원래대로 되돌림
// pcl::PointCloud -> sensor_msgs::PointCloud2
sensor_msgs::PointCloud2 pcl2sensor(pcl::PointCloud<pcl::PointXYZ> processed_msg)
{
  sensor_msgs::PointCloud2 raw_msg;
  pcl::toROSMsg(processed_msg, raw_msg);
  raw_msg.header.frame_id = "map";
  return raw_msg;
}

// 2D LiDAR도 사용할 가능성이 있으므로, LaserScan에 대한 처리 코드를 추가함
// sensor_msgs::LaserScan -> sensor_msgs::PointCloud2
sensor_msgs::PointCloud2 laser2sensor(sensor_msgs::LaserScan laser_msg)
{
  static laser_geometry::LaserProjection projector;
  sensor_msgs::PointCloud2 raw_msg;
  // 가독성을 위한 줄넘김이므로, 관례에 위반되지 않을 경우 다음과 같이 사용을 권장함
  projector.projectLaser(
    laser_msg, raw_msg, -1,
    laser_geometry::channel_option::Intensity | laser_geometry::channel_option::Distance
  );
  raw_msg.header.frame_id = "map";
  return raw_msg;
}

// pcl::PointCloud -> cv::Mat
void pcl2mat(pcl::PointCloud<pcl::PointXYZ>::Ptr pcl, cv::Mat &mat, int width, int height)
{
  int count = 0;

  if (!mat.empty())
  {
    mat.release();
  }
  mat.create(height, width, CV_32F);

  for (int v=0; v<mat.rows; v++)
  {
    for (int u=0; u<mat.cols; u++)
    {
      mat.at<float>(v, u) = pcl->points.at(count++).z * 1000;
    }
  }

  mat.convertTo(mat, CV_8U);
}

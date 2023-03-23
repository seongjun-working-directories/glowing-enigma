// *** 헤더 선언부 *** //
// ros, opencv에 해당하는 헤더 파일 목록
#ifndef CAMERA_POSE_ESTIMATOR_HPP
#define CAMERA_POSE_ESTIMATOR_HPP
#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"

// std에 해당하는 헤더 파일 목록
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

// *** 클래스 선언부 *** //
class CameraPoseEstimator
{
private:
  double* intrinsic_parameters;
  double* radial_tangential_distortion;
  cv::Mat camera_matrix, distortion_coefficients;
  cv::Size checkboard_dimensions;
  float square_size;
  int frames_per_second;
public:
  // constructor & destructor
  CameraPoseEstimator();
  ~CameraPoseEstimator();

  // methods
  int estimate_real_time_pose(bool change_intrinsic_parameters);
  bool process_estimation_logic(cv::Mat &frame, cv::Mat &draw_to_frame, std::vector<cv::Point2f> &found_corners, std::vector<cv::Point3f> &object_points, cv::Mat &rotation_vector, cv::Mat &translation_vector);
  void draw_axis(cv::Mat &raw_image, cv::Mat &processed_image, std::vector<cv::Point2f> &image_points, std::vector<cv::Point2f> &cornersSP_);
  void create_axis_points(std::vector<cv::Point3f> &axis);
  void create_world_coordinate_system(std::vector<cv::Point3f> &object_points);
  int calculate_intrinsic_parameters();
  bool save_intrinsic_parameters(std::string filename, cv::Mat calculated_camera_matrix, cv::Mat calculated_distortion_coefficients);
};
#endif
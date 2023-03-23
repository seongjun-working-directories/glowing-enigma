#include "ros/ros.h"
#include "depth_camera_extrinsic_calibrator/CameraCalibrator.hpp"

CameraCalibrator::CameraCalibrator() {
  board_size = cv::Size(8-1, 11-1);
  calibration_squared_dimensions = 0.02410f; // 단위: meter
};

CameraCalibrator::~CameraCalibrator() {

};
#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

class CameraCalibrator {
private:
  // variables
  std::vector<cv::Point2f> image_corners;
  cv::Size board_size;
  float calibration_squared_dimensions;
  bool found;

public:
  // constructor & destructor
  CameraCalibrator();
  ~CameraCalibrator();

  // functions
};
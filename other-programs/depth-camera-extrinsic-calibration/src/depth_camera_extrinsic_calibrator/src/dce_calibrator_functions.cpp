// *** 헤더 파일 목록 *** //
// ros, opencv에 해당하는 헤더 파일
#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/viz.hpp"
#include "opencv2/viz/vizcore.hpp"
#include "opencv2/viz/viz3d.hpp"

// std에 해당하는 헤더 파일
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// *** dce_calibrator에 사용되는 함수 목록 *** //
// 이미지에서 체크보드 코너를 찾은 뒤 해당 코너들을 이미지에 표시함
bool find_and_draw_chessboard_corners(
  cv::Mat &frame, cv::Mat &draw_to_frame, std::vector<cv::Vec2f> &found_points, const float calibration_squared_dimensions = 0.02410f, const cv::Size checkboard_dimensions = cv::Size(8-1, 11-1)
) {
  bool found = cv::findChessboardCorners(
    frame, checkboard_dimensions, found_points,
    cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK
  );
  cv::drawChessboardCorners(draw_to_frame, checkboard_dimensions, found_points, found);

  return found;
}

// 월드 좌표계를 저장하는 루프
void create_world_coordinate_system(
  std::vector<cv::Point3f> &object_points, const float calibration_squared_dimensions = 0.02410f, const cv::Size checkboard_dimensions = cv::Size(8-1, 11-1)
) {
  double row_start = 3 * calibration_squared_dimensions;
  double col_start = -4.5 * calibration_squared_dimensions;

  for (int col=0; col<checkboard_dimensions.height; col++) {
    for (int row=0; row<checkboard_dimensions.width; row++) {
      cv::Point3f temporary_3d_points(
        0.0f,
        col_start + col * calibration_squared_dimensions,
        row_start - row * calibration_squared_dimensions
      );
      object_points.push_back(temporary_3d_points);
      // [TEST] printf("temporary_3d_points(%d, %d) >>> %lf, %lf\n", col, row, temporary_3d_points.y, temporary_3d_points.z);
    }
  }
}

// 이미지 좌표계를 저장하는 루프
void create_image_coordinate_system(
  cv::Mat &frame, std::vector<cv::Point2f> &image_points, std::vector<cv::Vec2f> &found_points
) {
  for (std::vector<cv::Vec2f>::iterator found_points_iterator = found_points.begin(); found_points_iterator < found_points.end(); found_points_iterator++) {
    cv::Point2f temporary_2d_points((*found_points_iterator)[0] - frame.cols/2, (*found_points_iterator)[1] - frame.rows/2);
    image_points.push_back(temporary_2d_points);
    // [TEST] printf("temporary_2d_points >>> %lf, %lf\n", temporary_2d_points.x, temporary_2d_points.y);
  }
}

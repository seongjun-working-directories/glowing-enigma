// 헤더 파일 목록
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

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// 전역 상수 목록
const float calibration_squared_dimensions = 0.02410f; // 단위: meter
const cv::Size checkboard_dimensions = cv::Size(8-1, 11-1);

// 메인 함수 실행
int main(int argc, char **argv) {

  // 비디오 캡처를 위한 변수
  cv::VideoCapture vid(0);
  // 1초당 프레임 개수를 조절하기 위한 변수
  int frames_per_sec = 20;

  // 카메라에서 체크보드를 찾을 경우 보여줄 화면
  cv::namedWindow("Webcam - Found", cv::WINDOW_AUTOSIZE);
  // 카메라에서 체크보드를 찾지 못할 경우 보여줄 화면
  cv::namedWindow("Webcam - NOT Found", cv::WINDOW_AUTOSIZE);

  // 카메라 내부 파라미터를 double 타입 배열에 저장함
  // 순서 : fx, 0, cx, 0, fy, cy, 0, 0, 1
  double intrinsic_parameters[] = {453.7543640, 0.0, 323.54632568, 0.0, 453.7543640, 241.9261780, 0.0, 0.0, 1.0};
  // camera parameters in cv::Mat
  cv::Mat camera_matrix(3, 3, CV_64FC1, intrinsic_parameters);

  // 방사왜곡(radial distortion) 및 접선왜곡(tangential distortion)
  double radial_tangential_distortion[] = {0.0, 0.0, 0.0, 0.0};
  cv::Mat distortion_coefficients(4, 1, CV_64FC1, radial_tangential_distortion);

  // 비디오가 열려있지 않은 경우 프로그램을 종료함
  if (!vid.isOpened()) return -1;

  // 체크보드의 코너 검출에 성공했는지 여부를 표현하는 변수

  while (true) {
    cv::waitKey(1000/frames_per_sec);

    bool found = false;

    // 이미지를 저장하기 위한 변수
    cv::Mat frame, draw_to_frame;

    // 비디오에서 이미지를 읽어오지 못할 경우 프로그램을 종료
    if (!vid.read(frame)) break;

    // 이미지 원본과 체크보드 코너 검출을 표현한 이미지를 분리함
    frame.copyTo(draw_to_frame);

    // found_points는 보여준 패턴에서 검출된 내부 코너의 화소 좌표를 단순히 읽어온 값임
    std::vector<cv::Vec2f> found_points;

    // 이미지에서 체크보드 코너를 찾은 뒤 해당 코너들을 이미지에 표시함
    found = cv::findChessboardCorners(
      frame, checkboard_dimensions, found_points,
      cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK
    );
    cv::drawChessboardCorners(draw_to_frame, checkboard_dimensions, found_points, found);

    /* [TEST]
    */
    for (std::vector<cv::Vec2f>::iterator found_points_iterator = found_points.begin(); found_points_iterator < found_points.end(); found_points_iterator++) {
      printf("%lf %lf\n", (*found_points_iterator)[0], (*found_points_iterator)[1]);
    }

    // 코너가 검출된 경우 if문에서, 검출되지 않은 경우 else문에서 처리함
    if (found) {
      cv::imshow("Webcam - Found", draw_to_frame);
      // object_points는 월드좌표계를 저장함
      std::vector<cv::Point3f> object_points;
      // image_points는 이미지좌표계를 저장함
      std::vector<cv::Point2f> image_points;

      // 회전 벡터 및 이동 벡터를 저장하기 위한 변수
      cv::Mat rotation_vector, translation_vector;
      // 회전 벡터를 회전 행렬로 변환하여 저장하기 위한 변수로 cv::Rodrigues를 사용함
      cv::Mat rotation_matrix, inversed_rotation_matrix;

      // 월드 좌표계를 저장하는 루프
      double row_start, col_start;
      col_start = -4.5 * calibration_squared_dimensions;
      row_start = 3 * calibration_squared_dimensions;

      for (int col=0; col<checkboard_dimensions.height; col++) {
        for (int row=0; row<checkboard_dimensions.width; row++) {
          cv::Point3f temporary_3d_points(
            0.0f,
            col_start + col * calibration_squared_dimensions,
            row_start - row * calibration_squared_dimensions
          );
          object_points.push_back(temporary_3d_points);
          // [TEST]
          printf("temporary_3d_points(%d, %d) >>> %lf, %lf\n", col, row, temporary_3d_points.y, temporary_3d_points.z);
        }
      }

      // 이미지 좌표계를 저장하는 루프
      for (std::vector<cv::Vec2f>::iterator found_points_iterator = found_points.begin(); found_points_iterator < found_points.end(); found_points_iterator++) {
        cv::Point2f temporary_2d_points((*found_points_iterator)[0] - frame.cols/2, (*found_points_iterator)[1] - frame.rows/2);
        image_points.push_back(temporary_2d_points);
        // [TEST]
        printf("temporary_2d_points >>> %lf, %lf\n", temporary_2d_points.x, temporary_2d_points.y);
      }

      // cv::solvePnP()를 통해 내부 파라미터가 정적이라는 가정 하에 rotation_vector 및 translation_vector를 구함
      cv::solvePnP(object_points, image_points, camera_matrix, distortion_coefficients, rotation_vector, translation_vector);

      // rotation_matrix는 카메라의 방향 정보를 가져올 수 있는 변수
      cv::Rodrigues(rotation_vector, rotation_matrix);
      inversed_rotation_matrix = rotation_matrix.inv();

      // camera_position은 카메라의 위치 정보를 가져올 수 있는 변수
      cv::Mat camera_position = (-inversed_rotation_matrix) * translation_vector;
      double* camera_position_data = (double*)camera_position.data;

      // camera position
      printf("x=%lf, y=%lf, z=%lf\n", camera_position_data[0], camera_position_data[1], camera_position_data[2]);
    }
    else {
      cv::imshow("Webcam - NOT Found", frame);
    }
  }

  return 0;
}
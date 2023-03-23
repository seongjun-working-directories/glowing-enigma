// *** 헤더 파일 목록 *** //
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

#include "dce_calibrator_functions.cpp"

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

  while (true) {
    // 1초당 20프레임을 받을 수 있는 루프를 구현함
    cv::waitKey(1000/frames_per_sec);

    // 이미지를 저장하기 위한 변수
    cv::Mat frame, draw_to_frame;

    // 비디오에서 이미지를 읽어오지 못할 경우 프로그램을 종료
    if (!vid.read(frame)) break;

    // 이미지 원본과 체크보드 코너 검출을 표현한 이미지를 분리함
    frame.copyTo(draw_to_frame);

    // found_points는 보여준 패턴에서 검출된 내부 코너의 화소 좌표를 단순히 읽어온 값임
    std::vector<cv::Vec2f> found_points;

    // 이미지에서 체크보드 코너를 찾은 뒤 해당 코너들을 이미지에 표시함
    bool found = false;
    found = find_and_draw_chessboard_corners(frame, draw_to_frame, found_points);

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
      create_world_coordinate_system(object_points);
      // 이미지 좌표계를 저장하는 루프
      create_image_coordinate_system(frame, image_points, found_points);

      /* [TEST]
      */
      double customized_intrinsic_parameters[] = {3365.68, 0, 830.831, 0, 2723.72, 211.973, 0, 0, 1};
      cv::Mat customized_camera_matrix(3, 3, CV_64FC1, customized_intrinsic_parameters);
      double customized_distortion[] = {-0.454222, 1.41748, -0.0805329, -0.0492256, -1.20886};
      cv::Mat customized_distortion_coefficients(5, 1, CV_64FC1, radial_tangential_distortion);
      cv::solvePnP(object_points, image_points, customized_camera_matrix, customized_distortion_coefficients, rotation_vector, translation_vector);

      /* [TEST]
      // cv::solvePnP()를 통해 내부 파라미터가 정적이라는 가정 하에 rotation_vector 및 translation_vector를 구함
      cv::solvePnP(object_points, image_points, camera_matrix, distortion_coefficients, rotation_vetor, translation_vector);
      */

      std::vector<cv::Point2f> new_image_points;
      cv::projectPoints(object_points, rotation_vector, translation_vector, customized_camera_matrix, customized_distortion_coefficients, new_image_points);


      // cv::Mat axes_image;
      // draw_to_frame.copyTo(axes_image);
      // cv::drawFrameAxes(axes_image, customized_camera_matrix, distortion_coefficients, rotation_vector, translation_vector, 0.3);
      // cv::imshow("Axes Images", axes_image);

      // rotation_matrix는 카메라의 방향 정보를 가져올 수 있는 변수
      cv::Rodrigues(rotation_vector, rotation_matrix);
      inversed_rotation_matrix = rotation_matrix.inv();

      // rotation_matrix
      printf("ROTATION_MATRIX>>>\n");
      for(int y = 0; y < rotation_matrix.rows; ++y)
      {
        cv::Vec3f* rotation_matrix_row = rotation_matrix.ptr<cv::Vec3f>(y);
        for(int x = 0; x< rotation_matrix.cols; ++x)
        {
          std::cout << rotation_matrix_row[x] << " ";
        }
        std::cout << std::endl;
      }

      // translation_vector
      printf("TRANSLATION_VECTOR>>>\n");
      for(int y = 0; y < translation_vector.rows; ++y)
      {
        cv::Vec3f* translation_vector_row = translation_vector.ptr<cv::Vec3f>(y);
        for(int x = 0; x < translation_vector.cols; ++x)
        {
          std::cout << translation_vector_row[x] << " ";
        }
        std::cout << std::endl;
      }

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
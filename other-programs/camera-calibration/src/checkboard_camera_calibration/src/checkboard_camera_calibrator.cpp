#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/aruco.hpp"
#include "opencv2/core/hal/interface.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "checkboard_functions.cpp"

// 전역 상수
const float calibrationSquaredDimensions = 0.02410f; // 단위: meter
const float arucoSquareDimensions = 0.00f; // 단위: meter
const cv::Size checkboardDimensions = cv::Size(8-1, 11-1);

int main() {
  // [TEST] aruco_marker_creator();

  // [ Mat 클래스의 내장 함수 ]
  // -> .t() : Transpose 기능
  // -> .inv() : 역행렬을 반환
  // -> ::zeros(a, b, CV_OO) : a행 b열의 CV_OO 타입 영행렬을 반환
  // -> ::ones(a, b, CV_OO) :  a행 b열의 CV_OO 타입 모든 원소가 1인 행렬을 반환
  // -> ::eye(a, b, CV_OO) : a행 b열의 CV_OO 타입 단위 행렬(주대각선의 원소가 1, 나머지가 0인 행렬)을 반환

  cv::Mat frame, drawToFrame;
  cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
  cv::Mat distanceCoefficients;
  std::vector<cv::Mat> savedImages;
  std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

  // cv::VideoCapture 클래스
  // -> 동영상 입력 부분을 관리
  // cv::VideoCapture([filename | device]) 생성자
  // -> 동영상 파일명 또는 연결된 장치 index(하나인 경우 0을 입력)
  cv::VideoCapture vid(0);

  // cv::VideoCapture.isOpend() : returns true if video capturing has been initialized already
  if (!vid.isOpened()) { return -1; }

  int framesPerSec = 20;

  cv::namedWindow("Webcam - Found", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("Webcam - NOT Found", cv::WINDOW_AUTOSIZE);

  while (true) {
    // cv::VideoCapture.read() : 디바이스 장치로부터 frame을 하나씩 읽음
    if (!vid.read(frame)) { break; }  // 읽어온 frame이 없다면 루프를 종료

    bool found = false;

    // cv::Vec[채널수][자료형: b(byte), s(short), i(int), f(float), d(double)]
    std::vector<cv::Vec2f> foundPoints; // 평면상의 좌표들의 벡터

    // 주소를 공유하지 않고 독립적으로 동작하도록 drawToFrame에 복사하는 copyTo 메서드를 사용함
    frame.copyTo(drawToFrame);

    // cv::findChessboardCorners : finds the positions of internal corners of the chessboard
    // 반환 자료형 : bool
    // 파라미터 : InputArray image, Size patternSize, OutputArray corners, int flags=CALIB_CB_ADAPTIVE_THRESH+CALIB_CB_NORMALIZE_IMAGE
    found = cv::findChessboardCorners(
      frame, checkboardDimensions, foundPoints,
      cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK
    );

    cv::drawChessboardCorners(drawToFrame, checkboardDimensions, foundPoints, found);

    if (found) { cv::imshow("Webcam - Found", drawToFrame); }
    else { cv::imshow("Webcam - NOT Found", frame); }

    // 사용자로부터 키값을 입력받아 해당 키값에 따라 연산을 수행
    char character = cv::waitKey(1000/framesPerSec);

    switch(character) {
    // KEY `spacebar` :: 이미지를 벡터에 저장
    case ' ':
      if (found) {
        cv::Mat tmp;
        frame.copyTo(tmp);
        savedImages.push_back(tmp);
      }
      break;
    // KEY `enter` :: 카메라 캘리브레이션 파라미터 계산 및 저장
    case 13:
      if (savedImages.size() > 15) {
        cameraCalibration(savedImages, checkboardDimensions, calibrationSquaredDimensions, cameraMatrix, distanceCoefficients);
        saveCameraCalibration("parameters.txt", cameraMatrix, distanceCoefficients);
      }
      break;
    // KEY `esc` :: 프로그램 종료
    case 27:
      return 0;
    }
  }

  return 0;
}
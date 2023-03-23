// *** 헤더 파일 목록 *** //
#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// *** 전역 상수 *** //
const float calibrationSquaredDimensions = 0.02410f; // 단위: meter
const float arucoSquareDimensions = 0.00f; // 단위: meter
const cv::Size checkboardDimensions = cv::Size(8-1, 11-1);

// *** 체크보드의 코너값을 저장하는 함수  *** //
void getCheckboardCorners(
  std::vector<cv::Mat> images, std::vector<std::vector<cv::Point2f>>& allFoundCorners, bool showResults = false
) {
  // 루프를 돌며 각 이미지마다 연산을 처리
  for (std::vector<cv::Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
    std::vector<cv::Point2f> pointBuf;
    bool found = cv::findChessboardCorners(
      *iter, cv::Size(8-1, 11-1), pointBuf, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE
    );

    // cv::findChessboardCorners를 통해 찾아낸 Corner Points들을 allFoundCorners 변수에 저장
    if (found) {
      // [TEST] pointBuf에 코너 좌표가 잘 들어오는지 확인
      for (std::vector<cv::Point2f>::iterator iter = pointBuf.begin(); iter != pointBuf.end(); iter++) {
        ROS_INFO("pointBuf>>> (%f, %f)", (*iter).x, (*iter).y);
      }

      allFoundCorners.push_back(pointBuf);
    }
    if (showResults) {
      cv::drawChessboardCorners(*iter, cv::Size(8-1, 11-1), pointBuf, found);
      cv::imshow("Looking for corners", *iter);
      cv::waitKey(0);
    }
  }
}

// *** 체크보드의 실제 크기를 좌표값으로 저장하는 함수 *** //
void createKnownBoardPosition(
  cv::Size boardSize, float squaredEdgeLength, std::vector<cv::Point3f>& corners
) {
  for (int i=0; i<boardSize.height; i++) {
    for (int j=0; j<boardSize.width; j++) {
      corners.push_back(
        // 체크보드는 평면이기 때문에 z값이 0인 것이 특징
        cv::Point3f(j*squaredEdgeLength, i*squaredEdgeLength, 0.0f)
      );
    }
  }
}

// *** 카메라 파라미터 계수를 구하는 함수 *** //
void cameraCalibration(std::vector<cv::Mat> calibrationImages, cv::Size boardSize, float squareEdgeLength, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients) {
  // 촬영한 체크보드(calibrationImages)와 2차원 벡터(checkboardImageSpacePoints)를 인자로 전달
  // getCheckboardCorners 함수를 통해 checkboardImageSpacePoints에 각 이미지의 코너값을 저장함
  std::vector<std::vector<cv::Point2f>> checkboardImageSpacePoints;
  getCheckboardCorners(calibrationImages, checkboardImageSpacePoints, false);

  std::vector<std::vector<cv::Point3f>> worldSpaceCornerPoints(1);

  // 체크보드의 가로, 세로 및 각 칸의 길이, 3차원 좌표로 이뤄진 단일 벡터를 인자로 전달
  createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);

  // std::vector::size - https://cplusplus.com/reference/vector/vector/size/
  // std::vector::resize - https://cplusplus.com/rehttps://cplusplus.com/reference/vector/vector/resize/ference/vector/vector/resize/
  worldSpaceCornerPoints.resize(checkboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

  std::vector<cv::Mat> rVectors, tVectors;
  distanceCoefficients = cv::Mat::zeros(8, 1, CV_16F);

  // cv::calibrateCamera : finds the camera intrinsic & extrinsic parameters from serveral views of a calibration pattern
  // (링크 : https://amroamroamro.github.io/mexopencv/matlab/cv.calibrateCamera.html)
  cv::calibrateCamera(worldSpaceCornerPoints, checkboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficients, rVectors, tVectors);

  // rotation & translation vector 관련 자료는 `Rotate_Translate_Vector.pdf` 파일을 확인

  /*
  [ cv::calibrateCamera ]
  01. cameraMartix
  -> Input/output 3x3 floating-point camera intrinsic matrix A = ⎡ fx   0  cx  ⎤
                                                                 ⎢  0  fy  cy  ⎥
                                                                 ⎣  0   0   1  ⎦ .
  02. distanceCoefficients --> (k1, k2, p1, p2, k3)
  -> Input/output vector of distortion coefficients (k1,k2,p1,p2[,k3[,k4,k5,k6[,s1,s2,s3,s4[,τx,τy]]]]) of 4, 5, 8, 12 or 14 elements.
  */
}

// *** 카메라 파라미터 계수를 저장하는 함수 *** //
bool saveCameraCalibration(std::string name, cv::Mat cameraMatrix, cv::Mat distanceCoefficients) {
  std::ofstream outStream(name);
  if (outStream) {
    uint16_t rows = cameraMatrix.rows;
    uint16_t columns = cameraMatrix.cols;

    outStream << "cameraMatrix>>>" << std::endl;
    for (int r=0; r<rows; r++) {
      for (int c=0; c<columns; c++) {
        double value = cameraMatrix.at<double>(r, c);
        outStream << value << std::endl;
      }
    }

    rows = distanceCoefficients.rows;
    columns = distanceCoefficients.cols;

    outStream << "distanceCoefficients>>>" << std::endl;
    for (int r=0; r<rows; r++) {
      for (int c=0; c<columns; c++) {
        double value = distanceCoefficients.at<double>(r, c);
        outStream << value << std::endl;
      }
    }

    outStream.close();
    return true;
  }
  return false;
}

// *** 메인 함수 *** //
int main() {
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
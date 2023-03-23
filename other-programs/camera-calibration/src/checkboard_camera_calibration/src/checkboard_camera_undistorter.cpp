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
#include <string>
#include "checkboard_functions.cpp"

int main() {
  cv::Mat frame, undistortedFrame, map1, map2;
  cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
  cv::Mat distanceCoefficients = cv::Mat::zeros(1, 5, CV_64F);
  cv::Size imageSize = cv::Size(1280, 760);

  std::vector<std::vector<double>> cameraMatrixData;
  std::vector<std::vector<double>> distanceCoefficientsData;

  cv::VideoCapture vid(0);

  if (!vid.isOpened()) { return -1; }

  cv::namedWindow("Webcam - Undistorted Image", cv::WINDOW_AUTOSIZE);

  std::string line;
  std::ifstream file("parameters.txt");


  if (file.is_open()) {
    int count = 0;
    while(std::getline(file, line)) {
      // [TEST] std::cout << line << std::endl;
      if (count < 9) {
        cameraMatrix.at<double>((int)(count/3), (count%3)) = std::stod(line);
      }
      else {
        int distCoeffCount = count - 9;
        distanceCoefficients.at<double>(distCoeffCount, 0) = std::stod(line);
      }
      count++;
    }
    file.close();
  }

  cv::initUndistortRectifyMap(cameraMatrix, distanceCoefficients, cv::Mat(), cameraMatrix, imageSize, CV_32FC1, map1, map2);

  while (true) {
    if (!vid.read(frame)) { break; }

    // cv::undistort()는 initUndistortRectifyMap()과 remap()을 사용자가 편리하게 사용할 수 있도록 구현한 함수
    // -> cv::undistort()의 단점은 transformation matrix를 매번 구한다는 것
    // -> 그런데, 만약 카메라 보정 전후의 imageSize가 다르지 않다면, transformation matrix는 한 번만 구하면 됨
    // https://geniuskpj.tistory.com/19

    undistortedFrame = frame.clone();
    cv::remap(frame, undistortedFrame, map1, map2, cv::INTER_LINEAR);
    cv::imshow("Webcam - Undistorted Image", undistortedFrame);
    cv::waitKey(1);
  }

  return 0;
}
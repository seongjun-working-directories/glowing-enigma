// ===== Headers =====

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

// ===== Declarations =====

int pos = 128;

void thresholdByTrackBar(int, void*);

// ===== Main Function : 사용자 인터페이스(GUI) =====

int main() {

  cv::Mat f = cv::imread("assets/flower.jpg");

  if (f.empty()) {
    std::cout << "Error : " << std::endl;
    getchar();
    exit(0);
  }

  cv::cvtColor(f, f, cv::COLOR_BGR2GRAY);

  cv::namedWindow("Binarization");
  cv::createTrackbar("Thresholding", "Binarization", &pos, 255, thresholdByTrackBar, (void*)&f);
  cv::imshow("Binarization", f);

  cv::waitKey();

  return 0;
}

// ===== Definitions =====

void thresholdByTrackBar(int thresh, void* f) {
  cv::Mat* i = (cv::Mat*) f;
  cv::Mat bw;
  cv::threshold(*i, bw, thresh, 255, cv::THRESH_BINARY_INV);
  cv::imshow("Binarization", bw);
}
// ===== Headers =====

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"  // for `cvtColor()`
#include "iostream"

// ===== Declarations =====

void calc_histogram_gray(cv::Mat&, double[]);

cv::Mat draw_histogram_gray(double[]);

// ===== Main Function : 화소값을 일일이 읽어 히스토그램을 구하는 프로그램 =====

int main() {
  double hist[256];
  cv::Mat gray;
  cv::Mat img = cv::imread("assets/humming_bird.jpg");

  // 컬러 공간을 변환해주는 함수
  // COLOR_BGR2GRAY를 마지막 인자로 넘겨 컬러 영상 img를 명암 영상 gray로 변환함
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

  cv::namedWindow("Input Image");
  cv::imshow("Input Image", gray);

  calc_histogram_gray(gray, hist);
  cv::Mat bar = draw_histogram_gray(hist);
  cv::namedWindow("Histogram");
  cv::imshow("Histogram", bar);

  cv::waitKey();

  return 0;
}

// ===== Definitions =====

void calc_histogram_gray(cv::Mat& src, double hist[]) {
  int i, j, bin, ihist[256];

  // 배열을 0으로 초기화
  for (i=0; i<256; i++) ihist[i] = 0;

  if (src.channels() != 1) {
    std::cout << "ERR: 명암 영상을 입력하세요.";
  }
  else {
    for (j=0; j<src.rows; j++) {
      for (i=0; i<src.cols; i++) {
        // Mat::at => 영상의 픽셀값에 접근하기 위해 사용하는 함수
        bin = src.at<uchar>(j, i);
        // 각 픽셀에 접근하여 해당 픽셀이 가진 값을 인덱스로 한 ihist값에 1을 더함
        ihist[bin]++;
      }
    }

    for (i=0; i<256; i++) {
      // hist의 모든 요소의 합을 1로 만들기 위해 픽셀 개수만큼을 나눠 저장함
      hist[i] = ihist[i] / (double)(src.rows * src.cols);
    }
  }
}

cv::Mat draw_histogram_gray(double hist[]) {
  int i;
  double vmax = -1.0;

  // himg는 히스토그램을 그리기 위한 영상
  // Scalar 클래스는 영상의 픽셀 값(=행렬의 원소값)을 표현하는 용도로 주로 사용됨
  // cv::Scalar(255)를 사용해 바탕을 하얗게 만듦
  cv::Mat himg(256, 256, CV_8U, cv::Scalar(255));

  for (i=0; i<256; i++) {
    // vmax를 hist 내의 요소에서 가장 큰 값으로 설정함
    vmax = (vmax>hist[i]) ? vmax : hist[i];
  }

  for (i=0; i<256; i++) {
    cv::Point p1( i, 256 ); // column, row 순
    // 0.8 * 256의 의미는 막대가 영상 높이의 80%를 차지하도록 만들기 위함
    cv::Point p2( i, 256 - (int)((hist[i]/vmax)*(0.8*256)) );
    // himg라는 영상에 0이라는 값(검은 색)을 갖는 선분을 p1에서 p2까지 그려 넣음
    cv::line(himg, p1, p2, cv::Scalar(0));
  }

  return himg;
}
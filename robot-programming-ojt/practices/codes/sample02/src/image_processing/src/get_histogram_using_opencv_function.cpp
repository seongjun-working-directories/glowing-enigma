// ===== Headers =====
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

// ===== Declarations =====

// NO DECLARATION

// ===== Main Function : calcHist(), equalizeHist(), threshold()를 사용하는 프로그램 =====

int main() {
  cv::Mat img = cv::imread("assets/humming_bird.jpg");
  cv::Mat gray, res;

  // (a) 명암 영상
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
  cv::namedWindow("Contrast Image");
  cv::imshow("Contrast Image", gray);

  char choice = (char)cv::waitKey();

  // (b) 평활화된 영상
  if (choice == 'e') {
    // equalizeHist : 영상을 평활화하는 메서드
    cv::equalizeHist(gray, res);
    cv::namedWindow("Equalization Image");
    cv::imshow("Equalization Image", res);
  }
  // (c) 이진 영상
  else if (choice == 'b') {
    // threshold : 이진영상을 만드는 메서드
    // 인자 >>
    // (1) 입력할 변수(grayscale 이미지)
    // (2) 필터가 적용되어 저장될 이미지 변수
    // (3) 임계값(이진화시킬 기준 값)
    // (4) 임계값 이상의 픽셀들에 적용할 값
    // (5) 이진화 방법(threshold_type)
    cv::threshold(gray, res, 100, 255, cv::THRESH_BINARY);
    cv::namedWindow("Binary Image");
    cv::imshow("Binary Image", res);
  }
  // (d) 히스토그램(= 이미지에서 픽셀들이 가지는 값들의 출현빈도)
  else if (choice == 'h') {
    int i = 0;
    int channels[1] = {0};
    cv::MatND histo;
    int histsize[1] = {256};
    float hranges[2] = {0.0, 255.0};

    const float* ranges[1];
    ranges[0] = hranges;

    // calcHist : 이미지의 히스토그램 계산이 쉽도록 opencv가 제공하는 함수
    // 인자 >>
    // (1) const Mat*     images                히스토그램을 계산할 이미지들의 배열
    // (2) int            nimages               images 배열에 포함된 이미지 개수
    // (3) const int*     channels              히스토그램을 계산할 채널 번호들의 배열
    // (4) InputArray     mask                  히스토그램을 계산할 영역 --> Mat()을 전달하면 아무런 동작도 하지 않음
    // (5) OutputArray    hist                  히스토그램의 계산 결과를 저장함
    // (6) int            dims                  히스토그램의 계산 결과를 저장한 hist의 차원
    // (7) const int*     histSize              각 차원의 bin 개수 즉, 빈도수를 분류할 칸의 개수
    // (8) const float**  ranges                각 차원의 분류 bin의 최소값과 최대값
    // (9) bool           uniform = true
    // (10) bool          accumulate = true
    cv::calcHist(&gray, 1, channels, cv::Mat(), histo, 1, histsize, ranges);

    double vmax, vmin;
    cv::minMaxLoc(histo, &vmin, &vmax, 0, 0);

    res = cv::Mat::ones(256, 256, CV_8U) * 255;
    for (i=0; i<histsize[0]; i++) {
      cv::line(res, cv::Point(i, 256), cv::Point(i, 256-(int)((histo.at<float>(i)/vmax)*(0.8*256))), cv::Scalar(0));
    }
    cv::namedWindow("Histogram");
    cv::imshow("Histogram", res);
  }
  else {
    std::cout << "Error : ";
    exit(0);
  }

  std::vector<int> qlevel;
  // qlevel의 끝에 cv:IMWRITE_JPEG_QUALITY, 95를 추가
  qlevel.push_back(cv::IMWRITE_JPEG_QUALITY);
  qlevel.push_back(95);
  cv::imwrite("result.jpg", res, qlevel);

  cv::waitKey();

  return 0;
}

// ===== Definitions =====

// NO DEFINITIONS

/*
cf. threshold_type
  ■ THRESH_BINARY  :  이진화, threshold 값 이상 : 1, 이하 : 0(black) 으로 처리
  ■ THRESH_BINARY_INV  :  역 이진화, threshold 값 이상 : 0 (black), 이하 : 1 로 처리
  ■ THRESH_MASK : 흑색 이미지로 변경
  ■ THRESH_OTSU : Otsu 알고리즘 사용
  ■ THRESH_TOZERO : threshold 값 이상: 원본값, 이하 : 0
  ■ THRESH_TOZERO_INV : threshold 값 이상 : 0, 이하 : 원본값
  ■ THRESH_TRIANGLE : Triangle 알고리즘 사용
  ■ THRESH_TRUNC : threshold 값 이상 : threshold 값, 이하 : 원본값

cf. calcHist의 channels 인자 예시
img1(b=0, g=1, r=2), img2(b=3, g=4, r=5) 일때,  // 0~5는 채널 번호
첫 번째 이미지는 B 채널, 두 번째 이미지는 G 채널에 대해 히스토그램을 구하고자 한다면,
channels 인자에는 {0, 4}가 전달되어야 함
*/
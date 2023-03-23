// Object Detection with Histogram Backprojection using Color Distribution

// ===== Headers =====

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

// ===== Declarations =====

void calc_histogram(cv::Mat& src, double hist[]);

// ===== Main Function : 얼굴 검출 =====

int main() {
  // 1-(0) Get Input Image
  cv::Mat img = cv::imread("assets/2.png");
  cv::Mat hsv_img, hsv_histo_img;
  cv::Mat result_img;

  cv::Mat channels[3];
  std::vector<cv::Mat> channels_vector;

  cv::namedWindow("Original Image");
  cv::imshow("Original Image", img);

  char choice = (char)cv::waitKey();

  // 히스토그램 제작
  if (choice == '1') {
    double hist[256];
    // int i=0;
    // int channels[2] = {0, 1};
    // int histSize[2] = {256, 256};
    // float hranges[2] = {0.0, 256.0};
    // float sranges[2] = {0.0, 256.0};
    // const float* ranges[2] = {hranges, sranges};
    // cv::Mat intensity_excluded_img;

    // 1-(1) RGB to HSV, 1-(2) Exclude Intensity From HSV
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);

    cv::namedWindow("HSV Image");
    cv::imshow("HSV Image", hsv_img);

    // [TEST] std::cout << (hsv_img) << std::endl;

    // // 1-(2) Split & Merge Channels
    // // 위의 기능을 하도록 하려 했으나,
    // // cv::COLOR_BGR2HSV가 애초에 Intensity를 제외한 2개의 채널로 구성되기 때문에 필요가 없어진 코드
    // cv::split(hsv_img, channels);
    // channels_vector.push_back(channels[0]);
    // channels_vector.push_back(channels[1]);
    // cv::merge(channels_vector, intensity_excluded_img);
    // cv::namedWindow("HS Only Image");
    // cv::imshow("HS Only Image", intensity_excluded_img);

    // // 1-(3) Two Dimension Histogram 을 calcHist로 시도했으나 실패
    // // Reference : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=samsjang&logNo=220544731502
    // cv::calcHist(&hsv_img, 1, channels, cv::Mat(), hsv_histo_img, 2, histSize, ranges, true, false);
    // cv::normalize(hsv_histo_img, hsv_histo_img, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());
    // cv::invert(hsv_histo_img, hsv_histo_img, 1);
    // cv::calcBackProject(...);
    // double maxVal = 0; minMaxLoc(Hist, 0, &maxVal, 0, 0);
    // int scale = 10;
    // Mat histImage = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);
    // for(int i = 1; i < hbins * sbins; i++){
    //   line(histImage,
    //   Point(hbins*sbins*(i-1), sbins - cvRound(Hist.at<float>(i-1))),
    //   Point(hbins*sbins*(i-1), sbins - cvRound(Hist.at<float>(i))),
    //   Scalar(255,0,0), 2, 8, 0);
    // }
    // cv::namedWindow("HSV Histogram");
    // cv::imshow("HSV Histogram", hsv_histo_img);

    calc_histogram(hsv_img, hist);
  
    cv::waitKey();
  }
  else if (choice == '2') {

  }
  else {
    std::cout << "ERROR : Please check out your input value." << std::endl;
    exit(0);
  }

  // std::vector<int> qlevel;
  // qlevel.push_back(cv::IMWRITE_JPEG_QUALITY);
  // qlevel.push_back(95);
  // cv::imwrite("result.jpg", result_img, qlevel);

  cv::waitKey();

  return 0;
}

// ===== Definitions =====

void calc_histogram(cv::Mat& src, double hist[]) {
  int i, j, bin, ihist[256];

  for (i=0; i<256; i++) ihist[i] = 0;

  for (j=0; j<src.rows; j++) {
    for (i=0; i<src.cols; i++) {
      bin = src.at<uchar>(j, i);
      ihist[bin]++;
    }
  }

  for (i=0; i<256; i++) {
    hist[i] = ihist[i] / (double)(src.rows * src.cols);
  }
}

// Object Detection with Histogram Backprojection using Color Distribution

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

int main() {
  char choice;
  int i, j; // CAUTION!! ONLY USE WITH `FOR` LOOP

  cv::Mat raw_img = cv::imread("assets/after.png");
  cv::Mat raw_model = cv::imread("assets/before.png");
  cv::Mat img, model, hsv_img, hsv_model, result;

  // cv::GaussianBlur(raw_img, img, cv::Size(3, 3), 0);
  // cv::GaussianBlur(raw_model, model, cv::Size(3, 3), 0);

  img = raw_img.clone();
  model = raw_model.clone();

  cv::namedWindow("Original Image");
  cv::imshow("Original Image", raw_img);

  cv::namedWindow("Model Image");
  cv::imshow("Model Image", raw_model);

  choice = (char)cv::waitKey();

  if (choice == '1') {
    int h_hist[180] = { };
    int h_hist_max_frequence_value = 0;

    std::vector<cv::Mat> hsv_img_vectors;
    std::vector<cv::Mat> hsv_model_vectors;

    cv::Mat hsv_img_channels[3];
    cv::Mat hsv_model_channels[3];
    
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
    cv::cvtColor(model, hsv_model, cv::COLOR_BGR2HSV);

    cv::split(hsv_img, hsv_img_channels);
    cv::split(hsv_model, hsv_model_channels);

    for (i=0; i<180; i++) h_hist[i] = 0;

    for (j=0; j<hsv_model_channels[0].rows; j++) {
      for (i=0; i<hsv_model_channels[0].cols; i++) {
        h_hist[(int)(hsv_model_channels[0].at<uchar>(j, i))]++;
      }
    }

    /* [TEST]
    for (i=0; i<180; i++) std::cout << i << " : " << h_hist[i] << std::endl;
    */

    for (i=0; i<180; i++) {
      if (h_hist[h_hist_max_frequence_value] < h_hist[i]) {
        h_hist_max_frequence_value = i;
      }
    }

    /* [TEST]
    std::cout << "max value : " << h_hist_max_frequence_value << std::endl;
    */

    for (j=0; j<hsv_img_channels[0].rows; j++) {
      for (i=0; i<hsv_img_channels[0].cols; i++) {
        if (hsv_img_channels[0].at<uchar>(j, i) > h_hist_max_frequence_value - 5
          && hsv_img_channels[0].at<uchar>(j, i) < h_hist_max_frequence_value + 5) {
          hsv_img_channels[0].at<uchar>(j, i) = 255;
          hsv_img_channels[1].at<uchar>(j, i) = 255;
          hsv_img_channels[2].at<uchar>(j, i) = 255;
        }
        else {
          hsv_img_channels[0].at<uchar>(j, i) = 0;
          hsv_img_channels[1].at<uchar>(j, i) = 0;
          hsv_img_channels[2].at<uchar>(j, i) = 0;
        }
      }
    }

    hsv_img_vectors.push_back(hsv_img_channels[0]);
    hsv_img_vectors.push_back(hsv_img_channels[1]);
    hsv_img_vectors.push_back(hsv_img_channels[2]);

    cv::merge(hsv_img_vectors, result);

    cv::namedWindow("Result Image");
    cv::imshow("Result Image", result);
  }
  else {
    std::cout << "ERROR : Please check out your input value." << std::endl;
    exit(0);
  }

  cv::waitKey();

  return 0;
}
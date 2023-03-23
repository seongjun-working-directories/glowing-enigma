// Object Detection with Histogram Backprojection using Color Distribution

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "dummy_face_detector_function.cpp"

int pos = 128;

int main() {
  int i, j;

  cv::Mat img = cv::imread("assets/girl.jpg");
  cv::Mat hsv_img, h_img, s_img, inverted_img, result_img;

  double h_hist[180], s_hist[256];

  cv::namedWindow("Original Image");
  cv::imshow("Original Image", img);

  char choice = (char)cv::waitKey();

  if (choice == '1') {
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);

    inverted_img = invert_hue(img);
    cv::namedWindow("Inverted Image");
    cv::createTrackbar("Thresholding", "Inverted Image", &pos, 255, inverted_thresholdByTrackBar, (void*)&inverted_img);
    cv::imshow("Inverted Image", inverted_img);

    hsv_img = inverted_img;

    h_img = calc_hsv_histogram('h', hsv_img, h_hist);
    s_img = calc_hsv_histogram('s', hsv_img, s_hist);

    // cv::namedWindow("HSV Image");
    // cv::createTrackbar("Thresholding", "HSV Image", &pos, 255, hsv_thresholdByTrackBar, (void*)&hsv_img);
    // cv::imshow("HSV Image", hsv_img);

    // cv::namedWindow("H Image");
    // cv::createTrackbar("Thresholding", "H Image", &pos, 255, h_thresholdByTrackBar, (void*)&h_img);
    // cv::imshow("H Image", h_img);

    // cv::namedWindow("S Image");
    // cv::createTrackbar("Thresholding", "S Image", &pos, 255, s_thresholdByTrackBar, (void*)&s_img);
    // cv::imshow("S Image", s_img);

    cv::Mat channels[3];
    std::vector<cv::Mat> channels_vector;

    cv::split(hsv_img, channels);

    for (j=0; j<channels[0].rows; j++) {
      for (i=0; i<channels[0].cols; i++) {
        if (channels[0].at<uchar>(j, i) > 104) {
          channels[0].at<uchar>(j, i) = 255;
          channels[1].at<uchar>(j, i) = 255;
          channels[2].at<uchar>(j, i) = 255;
        }
        else {
          channels[0].at<uchar>(j, i) = 0;
          channels[1].at<uchar>(j, i) = 0;
          channels[2].at<uchar>(j, i) = 0;
        }
      }
    }

    channels_vector.push_back(channels[0]);
    channels_vector.push_back(channels[1]);
    channels_vector.push_back(channels[2]);

    cv::merge(channels_vector, result_img);

    // cv::Mat gray;
    // cv::cvtColor(result_img, gray, cv::COLOR_HSV2RGB);
    // cv::cvtColor(gray, gray, cv::COLOR_BGR2GRAY);

    // cv::namedWindow("GRAY");
    // cv::imshow("GRAY", gray);



    // // ====== LEGACY =====

    // /* [TEST]
    // std::cout << "\nh_hist>>>>>" << std::endl;
    // for (int i=0; i<(sizeof(h_hist)/sizeof(*h_hist)); i++) {
    //   std::cout << h_hist[i] << std::endl;
    // }
    // std::cout << "\ns_hist>>>>>" << std::endl;
    // for (int i=0; i<(sizeof(s_hist)/sizeof(*s_hist)); i++) {
    //   std::cout << s_hist[i] << std::endl;
    // }
    // */

    // std::vector<std::vector<cv::Scalar>> hs_values;

    // for (j=0; j<hsv_img.rows; j++) {
    //   std::vector<cv::Scalar> col_vals;
    //   for (i=0; i<hsv_img.cols; i++) {
    //     col_vals.push_back(cv::Scalar(h_img.at<uchar>(j, i), s_img.at<uchar>(j, i)));
    //   }
    //   hs_values.push_back(col_vals);
    // }

    // /* [TEST]
    // for (j=0; j<hsv_img.rows; j++) {
    //   for (i=0; i<hsv_img.cols; i++) {
    //     std::cout << hs_values[j][i][0] << " ";
    //   }
    //   std::cout << std::endl;
    // }
    // */

    // double hist[256][256];
    
    // for (j=0; j<256; j++) {
    //   for (i=0; i<256; i++) {
    //     hist[j][i] = 0;
    //   }
    // }

    // calc_hs_histogram(hs_values, hist);

    // /* [TEST]
    // for (j=0; j<256; j++) {
    //   for (i=0; i<256; i++) {
    //     std::cout << hist[j][i] << " ";
    //   }
    //   std::cout << std::endl;
    // }
    // */

    // std::vector<std::pair<double, double>> meaningful_value;

    // for (j=0; j<256; j++) {
    //   for (i=0; i<256; i++) {
    //     if (hist[j][i] < 247) {
    //       std::pair<double, double> tmp;
    //       tmp.first = j;
    //       tmp.second = i;
    //       meaningful_value.push_back(tmp);
    //     }
    //   }
    // }

    // for (i=0; i<meaningful_value.size(); i++) {
    //   std::cout << meaningful_value[i].first << ", " << meaningful_value[i].second << std::endl;
    // }

    // cv::inRange(inverted_img, cv::Scalar(3, 219, 0), cv::Scalar(15, 255, 255), result_img);

    cv::namedWindow("Result");
    cv::imshow("Result", result_img);
  }
  else if (choice == '2') {

  }
  else {
    std::cout << "ERROR : Please check out your input value." << std::endl;
    exit(0);
  }

  cv::waitKey();

  return 0;
}

#ifndef FACE_DETECTOR_FUNCTION_CPP
#define FACE_DETECTOR_FUNCTION_CPP

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

const int H_RANGE = 180;
const int S_RANGE = 256;

void calc_hs_histogram(std::vector<std::vector<cv::Scalar>> &hs_values, double hist[256][256]) {
  int i, j;

  for (j=0; j<256; j++) {
    for (i=0; i<256; i++) {
      hist[(int)((hs_values[j][i])[0])][(int)((hs_values[j][i])[1])]++;
    }
  }

  for (j=0; j<256; j++) {
    for (i=0; i<256; i++) {
      // hist[j][i] = hist[j][i]/(double)(256*256);
      hist[j][i] = 255 - hist[j][i]/(double)255;
    }
  }
}

cv::Mat calc_hsv_histogram(char mode, cv::Mat& src, double hist[]) {
  int i, j, bin;
  int *ihist;
  int channel_num;
  int channel_length;

  cv::Mat channels[3];
  cv::Mat chosen_channel;
  std::vector<cv::Mat> channels_vector;

  cv::split(src, channels);

  if (mode == 'h') {
    channels_vector.push_back(channels[0]);
    channel_length = H_RANGE;
  }
  else if (mode == 's') {
    channels_vector.push_back(channels[1]);
    channel_length = S_RANGE;
  }
  else {
    std::cout << "ERROR : Please check out your input value." << std::endl;
    exit(0);
  }

  ihist = (int*)malloc(channel_length * sizeof(int));

  for (i=0; i<channel_length; i++) ihist[i] = 0;

  cv::merge(channels_vector, chosen_channel);

  for (j=0; j<chosen_channel.rows; j++) {
    for (i=0; i<chosen_channel.cols; i++) {
      bin = chosen_channel.at<uchar>(j, i);
      ihist[bin]++;
    }
  }

  for (i=0; i<channel_length; i++) {
    hist[i] = ihist[i] / (double)(chosen_channel.rows * chosen_channel.cols);
  }

  free(ihist);

  return chosen_channel;
}

void hsv_thresholdByTrackBar(int thresh, void* f) {
  cv::Mat* i = (cv::Mat*) f;
  cv::Mat bw;
  cv::threshold(*i, bw, thresh, 255, cv::THRESH_BINARY_INV);
  cv:imshow("HSV_Binarization", bw);
}

void h_thresholdByTrackBar(int thresh, void* f) {
  cv::Mat* i = (cv::Mat*) f;
  cv::Mat bw;
  cv::threshold(*i, bw, thresh, 255, cv::THRESH_BINARY_INV);
  cv:imshow("H_Binarization", bw);
}

void s_thresholdByTrackBar(int thresh, void* f) {
  cv::Mat* i = (cv::Mat*) f;
  cv::Mat bw;
  cv::threshold(*i, bw, thresh, 255, cv::THRESH_BINARY_INV);
  cv:imshow("S_Binarization", bw);
}

void inverted_thresholdByTrackBar(int thresh, void* f) {
  cv::Mat* i = (cv::Mat*) f;
  cv::Mat bw;
  cv::threshold(*i, bw, thresh, 255, cv::THRESH_BINARY_INV);
  cv:imshow("Inverted_Binarization", bw);
}

cv::Mat invert_hue(cv::Mat& src) {
  int i, j;

  cv::Mat channels[3];
  cv::Mat hue_channel, inverted_img;
  std::vector<cv::Mat> channels_vector;

  cv::split(src, channels);

  hue_channel = channels[0];

  for (j=0; j<hue_channel.rows; j++) {
    for (i=0; i<hue_channel.cols; i++) {
      hue_channel.at<uchar>(j, i) = 360 - hue_channel.at<uchar>(j, i);
    }
  }

  channels_vector.push_back(hue_channel);
  channels_vector.push_back(channels[1]);
  channels_vector.push_back(channels[2]);

  cv::merge(channels_vector, inverted_img);

  return inverted_img;
}

#endif
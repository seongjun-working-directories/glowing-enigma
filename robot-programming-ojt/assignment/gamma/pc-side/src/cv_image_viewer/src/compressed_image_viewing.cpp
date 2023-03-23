#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.h"
#include "sensor_msgs/CompressedImage.h"
#include "image_transport/image_transport.h"
#include "iostream"

static const std::string OPENCV_WINDOW = "Compressed Image window 1";

class ImageConverter
{
  ros::NodeHandle nh;
  image_transport::ImageTransport imgTransport;

  ros::Subscriber imgCompSub;
  ros::Publisher imgCompPub;

  public:
  ImageConverter(): imgTransport(nh)
  {
    imgCompSub = nh.subscribe("/usb_cam/image_raw/compressed", 1, &ImageConverter::imageCb, this);
    imgCompPub = nh.advertise<sensor_msgs::CompressedImage>("/image_viewing/output_video", 1);
    cv::namedWindow(OPENCV_WINDOW);
  }
  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::CompressedImageConstPtr &msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      /*
      CompressedImage has header, format, and data.
      ex)
      header:
        seq: 2632
        stamp:
          secs: 1664445983
          nsecs: 820730634
        frame_id: "usb_cam"
      format: "rgb8; jpeg compressed bgr8"
      data: [255, 216, 255, 224, 0, 16, 74, 70, 73, 70, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 255, ...]
      */
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);


    }
    catch (cv_bridge::Exception &e)
    {
      ROS_ERROR("cv_bridge execption occurred>>> %s", e.what());
      return;
    }

    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(1);

    imgCompPub.publish(cv_ptr->toCompressedImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "compressed_image_viewing");
  ImageConverter imageConverter;
  ros::spin();

  return 0;
}
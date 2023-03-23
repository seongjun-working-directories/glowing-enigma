#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.h"
#include "image_transport/image_transport.h"
#include "iostream"

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh;
  image_transport::ImageTransport imgTransport;
  image_transport::Subscriber imgSub;
  image_transport::Publisher imgPub;

  public:
  // Constructor & Destructor
  ImageConverter(): imgTransport(nh)
  {
    // usb_cam 노드가 publishing 하는 image_raw를 subscribe
    imgSub = imgTransport.subscribe("/usb_cam/image_raw", 1, &ImageConverter::imageCb, this);
    imgPub = imgTransport.advertise("/image_converting/output_video", 1);
    cv::namedWindow(OPENCV_WINDOW);
  }
  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  // Methods
  // imageCb는 다음의 파라미터 형식에 전달됨 : void (ImageConverter::*fp)(const sensor_msgs::ImageConstPtr &)
  void imageCb(const sensor_msgs::ImageConstPtr &msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      // ROS의 sensor_msgs의 Image 클래스를 가리키는 포인터 senosr_msgs::ImageConstPtr과
      // 해당 이미지의 encoding 방식을 전달하면, cv_bridge::toCvCopy는 다음의 기능을 수행
      // Convert a sensor_msgs::Image message to an OpenCV-compatible CvImage, copying the image data.
      // 즉, sensor_msgs::Image의 data 속성을 해석해 CvImage로 변환함
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception &e)
    {
      ROS_ERROR("cv_bridge execption occurred>>> %s", e.what());
      return;
    }

    // http://wiki.ros.org/cv_bridge/Tutorials/UsingCvBridgeToConvertBetweenROSImagesAndOpenCVImages
    // 예제에는 원 그리기가 있지만, 필요없으므로 주석 처리 함
    // Draw an example circle on the video stream

    // if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
    // {
    //   cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));
    // }


    // Update GUI Window
    // CvImage로 변환된 이미지를 cv::imshow로 나타냄
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(1);

    // Output modified video stream
    // Convert this message to a ROS sensor_msgs::Image message. The returned sensor_msgs::Image message contains a copy of the image data.
    // 즉, CvImage로 변환된 이미지를 다시 ROS 형식에 맞게 sensor_msgs::Image로 변환함
    imgPub.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_viewing");
  ImageConverter imageConverter;
  ros::spin();

  return 0;
}
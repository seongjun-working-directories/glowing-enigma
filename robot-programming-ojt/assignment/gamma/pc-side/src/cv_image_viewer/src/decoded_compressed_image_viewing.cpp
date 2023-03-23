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

/*
[ cv::imdecode() ]
1. Mat cv::imdecode	(	InputArray buf, int flags )
2. Mat cv::imdecode ( InputArray buf, int flags, Mat* dst )

Reads an image from a buffer in memory.
The function imdecode reads an image from the specified buffer in the memory.
If the buffer is too short or contains invalid data, the function returns an empty matrix ( Mat::data==NULL ).
In the case of color images, the decoded images will have the channels stored in B G R order.

여기서 flags는 cv::imread에 들어가는 flags와 같음.

dst 매개변수
The optional output placeholder for the decoded matrix.
It can save the image reallocations when the function is called repeatedly for images of the same size.
*/

static const std::string OPENCV_WINDOW = "Compressed Image window 2";

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

      // cv_bridge::CompressedImage가 지원되지 않는다는 전제
      // 아래의 코드를 직접 구현
      // cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

      cv::Mat jpegData(1,msg->data.size(),CV_8UC1);
      jpegData.data = const_cast<uchar*>(&msg->data[0]);
      cv::InputArray data(jpegData);
      cv::Mat image = cv::imdecode(data,cv::IMREAD_COLOR);

      sensor_msgs::Image ros_image;
      ros_image.header = msg->header;
      ros_image.height = image.rows;
      ros_image.width = image.cols;
      ros_image.encoding = sensor_msgs::image_encodings::BGR8;
      ros_image.is_bigendian = false;
      ros_image.step = image.cols * image.elemSize();
      size_t size = ros_image.step * image.rows;
      ros_image.data.resize(size);
      if (image.isContinuous())
      {
        memcpy((char*)(&ros_image.data[0]), image.data, size);
      }
      else
      {
        uchar* ros_data_ptr = (uchar*)(&ros_image.data[0]);
        uchar* cv_data_ptr = image.data;
        for (int i = 0; i < image.rows; ++i)
        {
          memcpy(ros_data_ptr, cv_data_ptr, ros_image.step);
          ros_data_ptr += ros_image.step;
          cv_data_ptr += image.step;
        }
      }

      cv_ptr = cv_bridge::toCvCopy(ros_image, sensor_msgs::image_encodings::BGR8);
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
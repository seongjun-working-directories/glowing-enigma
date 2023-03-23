#include "ros/ros.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"  // imread(), namedWindow(), imshow()

int main() {
  // Mat은 영상을 표현하고 저장하는 데 사용하는 클래스
  cv::Mat img;

  cv::Mat g(512, 480, CV_8U, cv::Scalar(64));

  // 매개변수로 주어진 영상을 읽어들이고 Mat형을 반환함
  img=cv::imread("assets/humming_bird.jpg");

  // 글자를 삽입함
  // 인자 => 1번: 영상, 2번: 영상에 넣은 문자열, 3번: 문자를 쓸 위치, 나머지: 글자의 속성을 지정 
  cv::putText(img, "Hello, bird!", cv::Point(60, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 1, cv::LINE_AA, false);

  // 직사각형을 삽입
  // 인자 => 1번: 영상, 2번: 직사각형의 위치와 크기, 3번: 직사각형을 그리는 선의 RGB 값
  cv::rectangle(img, cv::Rect(30, 25, 350, 30), cv::Scalar(0, 0, 255));

  // 윈도우를 지정함
  cv::namedWindow("Hello");

  // 영상을 디스플레이함
  cv::imshow("Hello", img);

  // 창에 영상을 표시하는 시간(단위: 밀리초)
  cv::waitKey(5000);

  return 0;
}

/* [ Mat 클래스의 구현 ]

class CV_EXPORTS Mat {
  public:
  Mat(...);   // constructor(생성자)
  ~Mat(...);  // destructor(파괴자)
  Mat clone() const;
  void copyTo(Mat& m) const;
  void locateROI(Size& wholeSize, Point& ofs) const;  // ROI를 지정함
  template<typename_Tp> _Tp& at(int i0, i1);  // 화소에 접근하여 값을 읽고 씀

  ...

  int dims; // 영상의 차원 수
  int rows, cols; // 행과 열의 개수
  uchar *data;    // 영상을 저장하는 메모리 포인터
  int *refcount;  // 참조 횟수

  ...
}

Mat f, h; // 아무것도 정해지지 않은 빈 영상

// 512 X 480 크기의 unsigned 8비트 영상 g를 생성하고 64라는 값으로 초기화
Mat g(512, 480, CV_8U, Scalar(64));

// 512 X 480 크기의 세 채널을 가지는 unsigned 8비트 영상 img를 생성하고 빨간색으로 초기화
Mat img(Size(480, 512), CV_8UC3, Scalar(0, 0, 255));

Mat g1 = g; // refcount가 1만큼 증가하여, 2가 됨

img.copyTo(f);  //img의 복사본이 새로 생성되어 복사본을 f에 대입

h = img.clone();  // img의 복사본이 새로 생성되어 복사본을 h에 대입

*/

/* [ OpenCV의 기본 타입 ]

CV_8U : 8-bit unsigned integer: uchar ( 0..255 )
CV_8S : 8-bit signed integer: schar ( -128..127 )
CV_16U : 16-bit unsigned integer: ushort ( 0..65535 )
CV_16S : 16-bit signed integer: short ( -32768..32767 )
CV_32S : 32-bit signed integer: int ( -2147483648..2147483647 )
CV_32F : 32-bit floating-point number: float ( -FLT_MAX..FLT_MAX, INF, NAN )
CV_64F : 64-bit floating-point number: double ( -DBL_MAX..DBL_MAX, INF, NAN )

*/
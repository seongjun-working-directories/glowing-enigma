> ## TODO LIST

---
<br/>

> ### 1순위 intrinsic calibration으로 disCoeff 사용할 것
- ros_astra_camera 가 제시한 disCoeff(0.0, 0.0, 0.0, 0.0)은 문제가 있어 보임
- 최초 실행 시 disCoeff 값을 파일로 저장하고 특정 위치에 파일이 있는 경우 if-else로 건너뛰는 코드로 작성

<br/>

> ### 2순위 Dynamic TF Configure 적용
- rviz로 확인했을 때 결과값으로 나온 카메라의 위치를 실시간으로 확인할 수 있도록 Dynamic TF Configure를 적용해야 함
- urdf에는 고정된 장애물의 위치를 geometry로 표현해둘 것

<br/>

> ### 3순위 temporary_2d_points에 대한 normalization code 작성

'''\
import cv2 as cv

img = cv.imread(path)\
img = cv.resize(img, (800, 800))\
cv.normalize(img, img, 0, 255, cv.NORM_MINMAX)\

cv.imshow('dst_rt', img)\
cv.waitKey(0)\
cv.destroyAllWindows()\
'''

'''\
https://swprog.tistory.com/entry/OpenCV-normalize\
'''

<br/>

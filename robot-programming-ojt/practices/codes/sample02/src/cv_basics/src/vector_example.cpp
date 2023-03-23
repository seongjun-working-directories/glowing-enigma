// How to use `vector` in C++

/*
[ vector란? ]
C++ 표준 라이브러리 벡터 클래스는 시퀀스 컨테이너에 대한 클래스 템플릿
벡터는 지정된 형식의 요소를 선형 배열에 저장하고 모든 요소에 대한 빠른 임의 액세스를 허용함
임의 액세스 성능이 프리미엄인 경우 벡터는 시퀀스에 대한 기본 컨테이너

[ vector의 특징 ]
- heap 영역에 생성되어 동적할당됨

[ vector 초기화 ]
(1) vector<자료형>
  - 변수명	벡터 생성
  - ex) vector<int> v;

(2) vector<자료형> 변수명(숫자)
  - 숫자만큼 벡터 생성 후 0으로 초기화
  - ex) vector<int> v(4);

(3) vector<자료형> 변수명 = { 변수1, 변수2, 변수3... }
  - 벡터 생성 후 오른쪽 변수 값으로 초기화
  - ex) vector<int> v = {1, 2, 3};

(4) vector<자료형> 변수명[] = {, }
  - 벡터 배열(2차원 벡터)선언 및 초기화(열은 고정, 행은 가변)
  - ex) vector<int> v[] = {{1, 2}, {3, 4}}; // 행은 가변, 열은 고정

(5) vector<vector<자료형> 변수명
  - 2차원 벡터 생성(열과 행 모두 가변)
  - ex) vector<vector<int>> v;

(6) vector<자료형>변수명.assign(범위, 초기화할 값)
  - 벡터의 범위 내에서 해당 값으로 초기화
  - ex) vector<int> v = {1, 2, 3, 4, 5}; v.assign(5, 10); // output: 10 10 10 10 10

[ vector의 iterators ]
- v.begin() : 벡터 시작점의 주소 값 반환
- v.end() : 벡터 (끝부분 + 1) 주소값 반환
- v.rbegin() (revers begin) : 벡터의 끝 지점을 시작점으로 반환
- v.rend() (revers end) : 벡터의 (시작 + 1) 지점을 끝 부분으로 반환

ex)
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int main(){
  vector<int> v = { 1, 2, 3, 4 };

  for_each(v.begin(), v.end(), [&](int& n){
    cout << n << endl;        //output : 1 2 3 4
  });

  for_each(v.rbegin(), v.rend(), [&](int& n) {
    cout << n << endl;        //output : 4 3 2 1
  });

  vector<int>::iterator itor = v.begin();

  for (; itor != v.end(); itor++)
    cout << *itor << endl;        //output : 1 2 3 4

  vector<int>::reverse_iterator itor2 = v.rbegin();

  for (; itor2 != v.rend(); itor2++)
    cout << *itor2 << endl;        //output : 4 3 2 1

  return 0;
}​

[ vector element access(벡터의 요소 접근) ]
- v.at(i) : 백터의 i번째 요소 접근 (범위 검사함)
- v.[i] (operator []) : 백터의 i번째 요소 접근 (범위 검사 안함)
- v.front() : 백터의 첫번째 요소 접근
- v.back() : 백터의 마지막 요소 접근

ex)
vector<int> v = { 1, 2, 3, 4};

cout << v.front() << endl;        //output : 1
cout << v.back() << endl;        //output : 4
cout << v.at(1) << endl;        //output : 2
cout << v[2] << endl;            //output : 3​

[ vector element insertion(벡터의 요소 삽입) ]
- v.push_back() : 백터의 마지막 부분에 새로운 요소 추가
- v.pop_back() : 백터의 마지막 부분 제거
- v.insert(삽입할 위치의 주소 값, 변수 값) : 사용자가 원하는 위치에 요소 삽입
- v.emplace(삽입할 위치의 주소 값, 변수 값) : 사용자가 원하는 위치에 요소 삽입(move로 인해 복사생성자 X)
- v.emplace_back() : 백터의 마지막 부분에 새로운 요소 추가(move로 인해 복사생성자 X)
- v.erase(삭제할 위치) or v.erase(시작위치, 끝위치) : 사용자가 원하는 index값의 요소를 지움
- v.clear() : 백터의 모든 요소를 지운다.(return size = 0)
- v.resize(수정 값) : 백터의 사이즈를 조정(범위 초과시 0으로 초기화)
- v.swap(백터 변수) : 백터와 백터를 스왑
*/
#include <iostream>
#include <array> // 배열을 이용하기 위해서는 array를 추가해야함.
using namespace std;

template <size_t N> // 가변 크기 N 템플릿 생성
void print(const array<int, N>& arr) // print 함수 오버로드
{
	for (auto ele : arr) // auto는 지료형 자동 추론
		cout << ele << ", ";
}

int main() {
	array<int, 10> arr1; // 크기가 10인 int 타입 배열 선언
	arr1[0] = 1;
	cout << "arr1 배열의 첫 번째 원소 : " << arr1[0] << endl;

	array<int, 4> arr2 = { 1, 2, 3, 4 };
	cout << "arr2의 모든 원소 : ";
	for (int i = 0; i < arr2.size(); i++)
		cout << arr2[i] << " ";
	cout << endl;

	/*
		array 접근 시 [] 연산자를 제공한다.
		단, 빠른 접근을 위해 원소 인덱스가 배열의 크기에 맞는지는 검사하지 않는다.
		그래서 at() 함수를 지원함. 이 함수는 배열 크기 내인지 확인하고 예외 발생 가능.
	*/

	try
	{
		cout << arr2.at(3) << endl;
		cout << arr2.at(100) << endl;
	}
	catch (const out_of_range& ex)
	{
		cerr << ex.what() << endl; // cerr은 표준 에러 스트림, what()은 에러를 설명하는 스트림 반환
	}

	print(arr2);
	cout << endl;

	/*
		array는 begin(), end() 함수를 제공한다. 각각 첫 번째 원소와 마지막 원소 다음 위치를 반환한다.
	*/

	for (auto it = arr2.begin(); it != arr2.end(); it++)
	{
		auto element = (*it); // 요소 값 복사 발생, it은 마치 포인터처럼 작동하는 객체라고 보면 된다...
		cout << element << ' ';
	}
	cout << endl;

	/*
		진짜 포인터를 반환하려면 data() 함수를 이용하자. data()는 맨 첫번째 원소의 포인터를 반환함.
		front()는 첫 번째 원소에 대한 참조를 반환 -> 실제 값을 출력
		back()은 마지막 원소
	*/

	cout << arr2.front() << " " << arr2.back() << " " << *(arr2.data() + 1) << endl;
	// +1을 한 이유? int가 4바이트니까 +4라 생각할 수 있는데, +1은 해당 타입 1개를 의미하므로 자동으로 4가 된다.

}




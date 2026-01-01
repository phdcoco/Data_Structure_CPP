/*
	학교에서 학생 정보를 관리하는 응용 프로그램
	한 반의 학생 수와 학생 정보는 프로그램 실행 시 입력으로 받을 예정이다.
	학생 데이터는 배열과 유사한 자료구조를 사용할 것이고, 배열의 크기는 가변적이다.
	여러 반을 하나로 합칠 수 있어야 한다.
*/

#include <iostream>
#include <sstream>
#include <algorithm>

// using namespace std; 는 이제부터 되도록이면 쓰지 말자. name collision 위험.

template <typename T> // T는 자료형을 나중에 정하겠다.
class dynamic_array
{
private:
	T* data;
	size_t n; // size_t 자료형은 C/C++ 에서 제공, 메모리 크기와 인덱스를 표현하기 위해 만든 unsigned 정수형 타입, 음수를 표현할 수 없다!!

public:
	dynamic_array(int n) // 생성자
	{
		this->n = n; // this->n의 n은 클래스 멤버 변수 size_t n이고 그냥 n은 매개변수 n임.
		data = new T[n]; // 이제 어떤 자료형이든 배열을 생성할 수 있음. 템플릿 T를 통해서.
	}

	dynamic_array(const dynamic_array<T>& other) // 복사 생성자, 참조 전달(읽기만 함)
	{
		n = other.n; // 복사시킬 배열의 크기를 복사
		data = new T[n]; // 대상과 동일한 타입과 크기의 새 배열을 만든다.

		for (int i = 0; i < n; i++)
			data[i] = other[i]; // 복사한다.
	}

	/*
		[] 연산자를 제공한다. 범위 체크 없이 즉시 인덱스에 맞는 배열 원소에 접근한다.
		원소의 수정이 가능하다.
	*/
	T& operator[](int index)
	{
		return data[index];
	}

	const T& operator[](int index) const // const 버전은 오직 읽기 모드만 가능하다. 앞 const는 가져온 값 수정 불가, 뒤 const는 객체 타입 수정 불가
	{
		return data[index];
	}

	T& at(int index) // at함수를 제공한다. 범위 체크를 한 뒤 접근. 안전하나 느림.
	{
		if (index < n)
			return data[index];
		throw "Index out of range.";
	}

	size_t size() const // 배열 크기 반환, 반환형을 size_t 하는 것이 안정적.
	{
		return n;
	}

	~dynamic_array() // 메모리 해제
	{
		delete[] data;
	}

	/*
		begin() 함수 제공, 맨 첫 번째 원소를 가리키는 포인터가 나온다.
		a.begin() 은 0x1000
		*a.begin() 은 a.data[0]과 동일
		a.begin()[0] 역시 a.data[0]과 동일, 포인터 산술이 자동으로 일어난다.
	*/
	T* begin() { return data; } // 수정 가능
	const T* begin() const { return data; } // 읽기 전용
	T* end() { return data + n; }
	const T* end() const { return data + n; }

	/*
		두 배열을 하나로 합치는 함수를 friend로 선언한다.
		C++에 있는 friend 키워드를 쓰면 이 클래스의 private / protected 멤버에 접근할 수 있다.
		이 함수에서는 dynamic_array의 private 멤버에 접근이 가능하다.
		매개변수의 const는 원본 수정을 방지한다.
	*/
	friend dynamic_array<T> operator+(const dynamic_array<T>& arr1, const dynamic_array<T>& arr2)
	{
		dynamic_array<T> result(arr1.size() + arr2.size());
		std::copy(arr1.begin(), arr1.end(), result.begin());
		std::copy(arr2.begin(), arr2.end(), result.begin() + arr1.size()); // 이어 붙이기

		return result;
	}

	/*
		배열에 저장된 모든 데이터를 문자열로 반환하는 to_string() 함수 생성
		데이터 구분을 위한 문자열 sep을 인자로 받는다. 기본값은 ,(쉼표)로 지정한다.
		예를 들어 배열이 [1, 2, 3] 면 "1, 2, 3"으로 ,를 써서 출력해주겠다는 소리.
	*/
	std::string to_string(const std::string& sep = ", ")
	{
		if (n == 0)
			return "";
		std::ostringstream os; // ostringstream은 << 연산자로 값을 계속 넣어 하나의 string을 만드는 문자열 조립기다.
		os << data[0];

		for (int i = 1; i < n; i++)
			os << sep << data[i];

		return os.str(); // 완성된 string 뭉치들을 반환. str()는 내부 버퍼를 string으로 변환한다.
	}
};

struct student
{
	std::string name; // 학생 이름
	int standard; // 학급 정보(나이)
};

/*
	<< 연산자에 추가 기능 설정.
	추가 기능은 student 타입을 출력하는 기능.
	코드의 의미 : cout << 10; 에서 이제 << 뒤에 student로 선언된 객체가 등장하면 이 함수를 이용해 출력하라.
*/
std::ostream& operator<<(std::ostream& os, const student& s) // string뿐만 아니라 다른 타입도 받아야 되니까 ostream 씀.
{
	return (os << "[" << s.name << ", " << s.standard << "]");
}

int main()
{
	int nStudents; // 학생 수
	std::cout << "1반 학생 수를 입력하세요 : ";
	std::cin >> nStudents;

	dynamic_array<student> class1(nStudents); // student 구조체를 사용하는 배열 생성
	for (int i = 0; i < nStudents; i++)
	{
		std::string name; // 학생 이름
		int standard; // 나이
		std::cout << i + 1 << "번째 학생 이름과 나이를 입력하세요. : ";
		std::cin >> name >> standard;
		/*
			여기서 dynamic_array의 operator[] 호출, students& 배열 i번째 위치 반환
			student{name, standard} 임시 객체를 생성한다.
			student 객체가 class1[i] 위치에 복사 대입된다.
		*/
		class1[i] = student{ name, standard }; // 구조체 이용 시 {} 사용
	}

	/*
		class2 = class1을 하는 순간 컴파일러는 깊은 복사로 인식한다.
		기존 객체로 새 객체를 초기화하는 문장이기 때문이다.
		class2는 이미 존재하는게 아니라 지금 새로 생성되는거니까.
		만약 이미 있는 class2에 =를 쓰면 operator =를 호출할 것이다.
		컴파일러는 dynamic_array<student> class2(class1); 으로 변환한다.
		이제 dynamic_array(const dynamic_array<student>& other);을 호출하는데 여기서
		other는 class1, this는 class2이다.
	*/
	auto class2 = class1;
	std::cout << "1반을 복사하여 2반 생성: " << class2.to_string() << std::endl;

	// 두 학급을 합쳐서 새로운 큰 학급 생성
	auto class3 = class1 + class2; // dynamic_array의 operator + 를 호출한다.
	std::cout << "1반과 2반을 합쳐 3반 생성: " << class3.to_string() << std::endl;

	return 0;
}
#include "framework.h"
#include "Reflection.h"

// 가변 인자 템플릿을 사용하여 클래스 정의
// template<typename... Members>
// struct DynamicClass
// {
// 	// 멤버 변수들을 튜플로 저장
// 	std::tuple<Members...> members;
// 
// 	// 생성자로 멤버 변수들을 초기화
// 	DynamicClass(const Members&... args) : members(args...) {}
// 
// 	// 멤버 변수에 접근하기 위한 함수
// 	template<size_t N>
// 	auto& get()
// 	{
// 		return std::get<N>(members);
// 	}
// };
// 
// int main2()
// {
// 	// 런타임에 클래스의 멤버 변수 타입 결정
// 	using MyClass = DynamicClass<int, float>;
// 
// 	// 클래스 인스턴스 생성
// 	MyClass myObject(42, 3.14f);
// 
// 	// 멤버 변수에 접근하여 사용
// 	std::cout << "iValue: " << myObject.get<0>() << std::endl;
// 	std::cout << "fValue: " << myObject.get<1>() << std::endl;
// 	return 0;
// }


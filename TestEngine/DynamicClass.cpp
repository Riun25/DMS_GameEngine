#include "framework.h"
#include "Reflection.h"

// ���� ���� ���ø��� ����Ͽ� Ŭ���� ����
// template<typename... Members>
// struct DynamicClass
// {
// 	// ��� �������� Ʃ�÷� ����
// 	std::tuple<Members...> members;
// 
// 	// �����ڷ� ��� �������� �ʱ�ȭ
// 	DynamicClass(const Members&... args) : members(args...) {}
// 
// 	// ��� ������ �����ϱ� ���� �Լ�
// 	template<size_t N>
// 	auto& get()
// 	{
// 		return std::get<N>(members);
// 	}
// };
// 
// int main2()
// {
// 	// ��Ÿ�ӿ� Ŭ������ ��� ���� Ÿ�� ����
// 	using MyClass = DynamicClass<int, float>;
// 
// 	// Ŭ���� �ν��Ͻ� ����
// 	MyClass myObject(42, 3.14f);
// 
// 	// ��� ������ �����Ͽ� ���
// 	std::cout << "iValue: " << myObject.get<0>() << std::endl;
// 	std::cout << "fValue: " << myObject.get<1>() << std::endl;
// 	return 0;
// }


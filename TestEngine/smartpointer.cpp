#include "framework.h"

/// <summary>
/// std::shared_ptr의 레퍼런스 카운팅 테스트 코드입니다.
/// </summary>

class Obj;

class Comp
{
public:
	Comp()
	{
		std::cout << "Comp Const" << std::endl;
	}
	~Comp()
	{
		std::cout << "Comp Dest" << std::endl;
	}
	void func()
	{
		std::cout << "do something" << std::endl;
	}
	std::weak_ptr<Obj> m_owner;
};

class Obj
{
public:
	Obj()
	{
		std::cout << "Obj Const" << std::endl;
	}
	~Obj()
	{
		std::cout << "Obj Dest" << std::endl;
	}
	void func()
	{
		m_comp->func();
	}
	std::shared_ptr<Comp> m_comp;
};


int main()
{
	std::shared_ptr<Obj> newObj = std::make_shared<Obj>();
	std::shared_ptr<Comp> newComp = std::make_shared<Comp>();

	newObj->m_comp = newComp;
	newComp->m_owner = newObj;

	newObj->func();

	OutputDebugStringA("test");

	return 0;
}

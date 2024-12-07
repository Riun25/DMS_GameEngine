#include "framework.h"

struct A
{
	A()
	{
		std::cout << "A const" << std::endl;
	}
	virtual ~A()
	{
		std::cout << "A dest" << std::endl;
	}
	virtual void DoSomething()
	{
		std::cout << "A" << std::endl;
	}
};

struct B
{
	B()
	{
		std::cout << "B const" << std::endl;
	}
	virtual ~B()
	{
		std::cout << "B dest" << std::endl;
	}
	virtual void DoSomething()
	{
		std::cout << "B" << std::endl;
	}
};

struct C : public A, public B
{
	C()
	{
		std::cout << "C const" << std::endl;
	}
	~C()
	{
		std::cout << "C dest" << std::endl;
	}
};



int main()
{
	C c;


	return 0;
}
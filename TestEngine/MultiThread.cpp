#include "framework.h"

/// <summary>
/// 간단한 멀티 스레드 테스트 코드입니다.
/// </summary>

void Func1(float dT)
{
	cout << "Func1()\n";
}

void Func2(float dT)
{
	cout << "Func2()\n";
}

void Func3(float dT)
{
	cout << "Func3()\n";
}

int main4()
{
	float dT = 1 / 60;


	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::thread t1(Func1, std::ref(dT));
		std::thread t2(Func2, std::ref(dT));
		std::thread t3(Func3, std::ref(dT));

		t1.join();
		t2.join();
		t3.join();
	}
	cout << "main()\n";

	return 0;
}
#include "framework.h"

/// std::shared_ptr의 복사 연산과 이동 연산의 연산 시간 차이를 비교하는 테스트 코드입니다.

// 타이머 클래스 정의
class Timer {
public:
	Timer() : start_time_point(std::chrono::high_resolution_clock::now()) {}

	~Timer() {
		Stop();
	}

	void Stop() {
		auto end_time_point = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time_point).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		std::cout << duration << "us (" << ms << "ms)\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point;
};

class Object {
public:
	Object() = default;
	~Object() = default;
};

void TestSharedPtrCopy(size_t num_operations) {
	std::vector<std::shared_ptr<Object>> vec1;
	vec1.reserve(num_operations);

	auto original = std::make_shared<Object>();
	for (size_t i = 0; i < num_operations; ++i) {
		vec1.push_back(original); // 초기 std::shared_ptr 벡터에 채우기
	}

	std::vector<std::shared_ptr<Object>> vec2;
	vec2.reserve(num_operations);

	{
		Timer timer;
		for (size_t i = 0; i < num_operations; ++i) {
			vec2.push_back(vec1[i]); // std::shared_ptr 복사
		}
	}
}

void TestSharedPtrMove(size_t num_operations) {
	std::vector<std::shared_ptr<Object>> vec1;
	vec1.reserve(num_operations);

	for (size_t i = 0; i < num_operations; ++i) {
		vec1.push_back(std::make_shared<Object>()); // 초기 std::shared_ptr 벡터에 채우기
	}

	std::vector<std::shared_ptr<Object>> vec2;
	vec2.reserve(num_operations);

	{
		Timer timer;
		for (size_t i = 0; i < num_operations; ++i) {
			vec2.push_back(std::move(vec1[i])); // std::shared_ptr 이동
		}
	}
}

int main() {
	size_t num_operations = 1000000;

	std::cout << "Testing std::shared_ptr copy...\n";
	TestSharedPtrCopy(num_operations);

	std::cout << "Testing std::shared_ptr move...\n";
	TestSharedPtrMove(num_operations);

	return 0;
}
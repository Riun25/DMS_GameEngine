#include "framework.h"

#include <iostream>
#include <optional>
#include <string>

std::optional<std::string> GetUsername(bool valid) 
{
	if (valid) {
		return "Alice";
	}
	else {
		return std::nullopt;
	}
}

int main() {
	auto username = GetUsername(true);

	if (username) {
		std::cout << "Username: " << *username << std::endl;
	}
	else {
		std::cout << "No valid username found." << std::endl;
	}

	username = GetUsername(false);

	std::cout << "Username: " << username.value_or("DefaultUser") << std::endl;

	return 0;
}

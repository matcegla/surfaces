#ifndef SURFACES_LG_HPP
#define SURFACES_LG_HPP

#include <iostream>
#include <glm/vec2.hpp>

extern struct Log {
	template<typename... Ts> void error(Ts&&... xs) { std::cout << "\033[1;31merror:\033[0m "; (std::cout << ... << xs); }
	template<typename... Ts> void info(Ts&&... xs) { std::cout << "info: "; (std::cout << ... << xs); }
} lg;

namespace glm {
	std::ostream &operator<<(std::ostream &out, const glm::vec2& v);
}

#endif //SURFACES_LG_HPP

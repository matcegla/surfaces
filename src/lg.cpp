#include "lg.hpp"

Log lg;

namespace glm {
	std::ostream& operator<<(std::ostream& out, const glm::vec2& v) {
		return out << v.x << ',' << v.y;
	}
}

#ifndef SURFACES_LG_HPP
#define SURFACES_LG_HPP

#include <glm/vec2.hpp>
#include <iostream>
#include <vector>

extern struct Log {
  template <typename... Ts> void error(Ts &&... xs) {
    std::cout << "\033[1;31merror:\033[0m ";
    (std::cout << ... << xs);
  }
  template <typename... Ts> void info(Ts &&... xs) {
    std::cout << "info: ";
    (std::cout << ... << xs);
  }
} lg;

namespace glm {
std::ostream &operator<<(std::ostream &out, const glm::vec2 &v);
}
template <typename T>
inline std::ostream &operator<<(std::ostream &out, const std::vector<T> &xs) {
  out << "[";
  for (auto i = 0; i < (int)xs.size() - 1; ++i)
    out << xs[i] << ", ";
  if (not xs.empty())
    out << xs.back();
  return out << ']';
}

#endif // SURFACES_LG_HPP

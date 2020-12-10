#include "helpers.hpp"

#include <string>

bool IsInt(std::string_view s) {
  for (const char c : s) {
    if (!isdigit(c))
      return false;
  }

  return true;
}

uint32_t IndexAlphabet(std::string_view name) noexcept {
  assert(name.size() >= 2);
  assert(name[0] >= 'A');
  assert(name[0] <= 'Z');

  return name[0] - 'A';
}

uint32_t IndexInt(std::string_view name) noexcept {
  assert(name.size() >= 2);

  std::string int_index(name.begin() + 1, name.end());
  return atoi(int_index.c_str());
}

std::string IndexToAlphabet(uint32_t a_index) noexcept {
  char c = a_index + 'A';
  assert(c >= 'A');
  assert(c <= 'Z');
  return std::string(1, c);
}

std::string IndexToInt(uint32_t i_index) noexcept {
  return std::to_string(i_index);
}

std::string ReferenceToString(const Reference& r) noexcept {
  return IndexToAlphabet(r.a_index) + IndexToInt(r.i_index);
}

uint32_t BlockIndex(uint32_t i_index) noexcept {
  return i_index / BlockDimI;
}
uint32_t InBlockIndex(uint32_t i_index) noexcept {
  return i_index % BlockDimI;
}
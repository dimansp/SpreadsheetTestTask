#include "cell_value.hpp"

#include <ostream>
#include <assert.h>

bool IsEmpty(const CellValue& v) {
  return v.index() == 0;
}
bool IsInvalid(const CellValue& v) {
  return v.index() == 1;
}
bool IsInt(const CellValue& v) {
  return v.index() == 2;
}

std::ostream& operator <<(std::ostream& stream, const CellValue& v) {
  if (IsEmpty(v)) {
    stream << "";
    return stream;
  }
  if (IsInvalid(v)) {
    stream << "Invalid";
    return stream;
  }
  if (IsInt(v)) {
    stream << std::get<int>(v);
    return stream;
  }
  assert(false);
  return stream;
}

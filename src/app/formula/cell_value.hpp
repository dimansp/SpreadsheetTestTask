#pragma once

#include <variant>
#include <iosfwd>

struct EmptyValue {};
struct InvalidValue {};

/* 
  TODO: Add other types: double, string, datetime and so on... 
*/
using CellValue = std::variant<EmptyValue, InvalidValue, int>;

bool IsEmpty(const CellValue& v);
bool IsInvalid(const CellValue& v);
bool IsInt(const CellValue& v);

std::ostream& operator <<(std::ostream& stream, const CellValue& v);

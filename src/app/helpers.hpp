#pragma once

#include <string_view>
#include <cinttypes>

struct Reference {
  Reference(uint32_t i = 0, uint32_t a = 0)
    : i_index(i)
    , a_index(a) {}

  uint32_t i_index;
  uint32_t a_index;
};

template<uint32_t I, uint32_t A>
class CellsBlock;

constexpr uint32_t BlockDimI = 64;
constexpr uint32_t BlockDimA = 'Z' - 'A';

using Block = CellsBlock<BlockDimI, BlockDimA>;

bool IsInt(std::string_view s);

uint32_t IndexAlphabet(std::string_view name) noexcept;
uint32_t IndexInt(std::string_view name) noexcept;

std::string IndexToAlphabet(uint32_t a_index) noexcept;
std::string IndexToInt(uint32_t i_index) noexcept;
std::string ReferenceToString(const Reference& r) noexcept;

uint32_t BlockIndex(uint32_t i_index) noexcept;
uint32_t InBlockIndex(uint32_t i_index) noexcept;

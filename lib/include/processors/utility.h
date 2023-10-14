#pragma once
#include <cstdint>
#include <iostream>
#include <string>

namespace task::utility {
template <class Elem, class Traits>
void hex_dump(const void *aData, std::size_t aLength,
              std::basic_ostream<Elem, Traits> &aStream,
              std::size_t aWidth = 16);

} // namespace task::utility

#include "processors/utility.hpp"
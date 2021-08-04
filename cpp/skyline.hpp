#ifndef PALLET_PACKING_CPP_SKYLINE_HPP
#define PALLET_PACKING_CPP_SKYLINE_HPP

#include <span>

#include "common.h"

int skyline_decode(int pallet_width, int pallet_height, std::span<Item> items, std::span<size_t> permutation,
                   std::span<Position> results);

#endif //PALLET_PACKING_CPP_SKYLINE_HPP

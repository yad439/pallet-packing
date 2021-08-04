#ifndef PALLET_PACKING_CPP_LIBRARY_H
#define PALLET_PACKING_CPP_LIBRARY_H

#include "common.h"

extern "C" {
[[maybe_unused]] __declspec(dllexport) int __cdecl simple_skyline(
		int pallet_width, int pallet_height, int n, Item *items, Position *positions
                                                                 );

[[maybe_unused]] __declspec(dllexport) int __cdecl simulated_annealing_skyline(
		int pallet_width, int pallet_height, int n, Item *items, Position *positions
                                                                              );
}

#endif //PALLET_PACKING_CPP_LIBRARY_H

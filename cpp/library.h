#ifndef PALLET_PACKING_CPP_LIBRARY_H
#define PALLET_PACKING_CPP_LIBRARY_H

#include "common.h"

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#define __declspec(v)
#define CALL_PREF
#ifdef defined(__GNUC__) && defined(__linux__)
#define CALL_SUF
#else
#define CALL_SUF __attribute__((cdecl))
#endif
#else
#define CALL_PREF __cdecl
#define CALL_SUF __attribute__((cdecl))
#endif

extern "C" {
[[maybe_unused]] __declspec(dllexport) int CALL_PREF simple_skyline(
		int pallet_width, int pallet_height, unsigned n, const Item *items, Position *positions
                                                                   )  CALL_SUF;

[[maybe_unused]] __declspec(dllexport) int CALL_PREF simulated_annealing_skyline(
		int pallet_width, int pallet_height, unsigned n, const Item *items, Position *positions,
		unsigned steps, unsigned same_temperature_steps, double start_temperature, double power
                                                                                )CALL_SUF;

[[maybe_unused]] __declspec(dllexport) int CALL_PREF simulated_annealing_skyline_auto(
		int pallet_width, int pallet_height, unsigned n, const Item *items, Position *positions
                                                                                     )CALL_SUF;
}

#endif //PALLET_PACKING_CPP_LIBRARY_H

#include <ranges>
#include <vector>
#include <numeric>

#include "skyline.hpp"

using std::span;

extern "C" {
[[maybe_unused]] __declspec(dllexport) int __cdecl simple_skyline(
		int pallet_width, int pallet_height, int n, Item *items, Position *positions
                                                                 ) noexcept {
	std::vector<size_t> permutation(static_cast<std::vector<size_t>::size_type>(n));
	std::iota(permutation.begin(), permutation.end(), 0);
	return skyline_decode(pallet_width, pallet_height, span(items, static_cast<std::span<Item>::size_type>(n)),
	                      span(permutation), span(positions, static_cast<std::span<Position>::size_type>(n)));
}
}
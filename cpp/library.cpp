#include "library.h"

#include <ranges>
#include <vector>
#include <numeric>
#include <chrono>
#include <cmath>

#include "skyline.hpp"
#include "annealing.hpp"
#include "permutation_encoding.hpp"

using std::span;
using namespace modular;

extern "C" {

[[maybe_unused]] int __cdecl simple_skyline(
		int pallet_width, int pallet_height, int n, Item *items, Position *positions
                                           ) {
	std::vector<size_t> permutation(static_cast<std::vector<size_t>::size_type>(n));
	std::iota(permutation.begin(), permutation.end(), 0);
	return skyline_decode(pallet_width, pallet_height, span(items, static_cast<std::span<Item>::size_type>(n)),
	                      span(permutation), span(positions, static_cast<std::span<Position>::size_type>(n)));
}

[[maybe_unused]] int __cdecl simulated_annealing_skyline(
		int pallet_width, int pallet_height, int n, Item *items, Position *positions
                                                        ) {
	const auto start_temperature = pallet_width * pallet_height / 2;
	const auto steps = 100000;
	const auto power = pow(-start_temperature * log(1e-3), -1.0 / steps);
	std::default_random_engine rng(
			static_cast<std::default_random_engine::result_type>(
					std::chrono::high_resolution_clock::now().time_since_epoch().count()
			)
	);
	std::uniform_real_distribution<double> dist;
	const auto settings = AnnealingSettings(
			steps, 1, start_temperature,
			[power](const auto x) { return x * power; },
			[&rng, &dist](const auto old_solution, const auto new_solution, const auto temp) {
				return dist(rng) < exp(static_cast<double>(old_solution - new_solution) / temp);
			}
	);

	std::vector<size_t> permutation(static_cast<std::vector<size_t>::size_type>(n));
	std::iota(permutation.begin(), permutation.end(), 0);
	std::shuffle(permutation.begin(), permutation.end(), rng);

	const auto resulting_encoding = annealing(
			settings,
			std::function(
					[pallet_width, pallet_height, items = span(items, static_cast<std::span<Item>::size_type>(n))]
							(const PermutationEncoding &enc) {
						return skyline_decode(pallet_width, pallet_height, items, span(enc.permutation()),
						                      span<Position>());
					}),
			PermutationEncoding(permutation),
			rng
	);

	return skyline_decode(pallet_width, pallet_height, span(items, static_cast<std::span<Item>::size_type>(n)),
	                      span(resulting_encoding.permutation()),
	                      span(positions, static_cast<std::span<Position>::size_type>(n)));
}

}

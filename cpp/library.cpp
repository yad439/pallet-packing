#include "library.h"

#include <ranges>
#include <vector>
#include <numeric>
#include <chrono>
#include <cmath>

#include "skyline.hpp"
#include "annealing.hpp"
#include "permutation_encoding.hpp"

using namespace std;

extern "C" {

[[maybe_unused]] int __cdecl simple_skyline(
		const int pallet_width, const int pallet_height, const unsigned n, const Item *const items,
		Position *const positions
                                           ) {
	vector<size_t> permutation(n);
	iota(permutation.begin(), permutation.end(), 0);
	return skyline_decode(pallet_width, pallet_height, span(items, n), span(permutation), span(positions, n));
}

[[maybe_unused]] int __cdecl simulated_annealing_skyline(
		const int pallet_width, const int pallet_height, const unsigned n, const Item *const items,
		Position *const positions,
		const unsigned steps, const unsigned same_temperature_steps, const double start_temperature, const double power
                                                        ) {
	default_random_engine rng(
			static_cast<default_random_engine::result_type>(
					chrono::high_resolution_clock::now().time_since_epoch().count()
			)
	);
	uniform_real_distribution<double> dist;
	const auto settings = AnnealingSettings(
			steps, same_temperature_steps, start_temperature,
			[power](const auto x) { return x * power; },
			[&rng, &dist](const auto old_solution, const auto new_solution, const auto temp) {
				return dist(rng) < exp(static_cast<double>(old_solution - new_solution) / temp);
			}
	);

	vector<size_t> permutation(n);
	iota(permutation.begin(), permutation.end(), 0);
	shuffle(permutation.begin(), permutation.end(), rng);

	const auto resulting_encoding = annealing(
			settings,
			function(
					[pallet_width, pallet_height, items = span(items, n)]
							(const PermutationEncoding &enc) {
						return skyline_decode(pallet_width, pallet_height, items, span(enc.permutation()),
						                      span<Position>());
					}),
			PermutationEncoding(permutation),
			rng
	);

	return skyline_decode(pallet_width, pallet_height, span(items, n), span(resulting_encoding.permutation()),
	                      span(positions, n));
}

[[maybe_unused]] int __cdecl simulated_annealing_skyline_auto(
		const int pallet_width, const int pallet_height, const unsigned n, const Item *const items,
		Position *const positions
                                                             ) {
	const auto start_temperature = pallet_width * pallet_height / 2;
	constexpr auto steps = 100000;
	constexpr auto same_temperature_steps = 1;
	const auto power = pow(-start_temperature * log(1e-3), -1.0 / steps);
	return simulated_annealing_skyline(pallet_width, pallet_height, n, items, positions, steps, same_temperature_steps,
	                                   start_temperature, power);
}

}

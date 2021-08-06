#ifndef PALLET_PACKING_CPP_ANNEALING_HPP
#define PALLET_PACKING_CPP_ANNEALING_HPP

#include <functional>
#include <utility>
#include <random>

struct AnnealingSettings {
	const unsigned iterations;
	const unsigned same_temperature_iterations;
	const double start_temperature;
	const std::function<double(double)> decreasing_function;
	const std::function<bool(int, int, double)> should_change;

	AnnealingSettings(const unsigned int iterations, const unsigned int sameTemperatureIterations,
	                  const double startTemperature, std::function<double(double)> decreasingFunction,
	                  std::function<bool(int, int, double)> shouldChange)
			: iterations(iterations), same_temperature_iterations(sameTemperatureIterations), start_temperature(
			startTemperature), decreasing_function(std::move(decreasingFunction)), should_change(
			std::move(shouldChange)) {}
};

template<class Encoding>
[[nodiscard]] Encoding annealing(const AnnealingSettings settings /*NOLINT(performance-unnecessary-value-param)*/,
                                 const std::function<int(const Encoding &)> score_function,
                                 Encoding start_encoding, std::default_random_engine &rng) {
	auto current_solution = std::move(start_encoding);
	auto min_score = score_function(current_solution);
	auto min_solution = current_solution;

	auto previous_score = min_score;
	auto temperature = settings.start_temperature;
	for (unsigned temp_iter = 0; temp_iter < settings.iterations; ++temp_iter) {
		for (unsigned inner_iter = 0; inner_iter < settings.same_temperature_iterations; ++inner_iter) {
			auto change = current_solution.random_change(rng);
			auto new_score = score_function(current_solution);

			if (settings.should_change(previous_score, new_score, temperature)) {
				previous_score = new_score;
			} else {
				current_solution.restore(change);
			}

			if (new_score < min_score) {
				min_score = new_score;
				min_solution = current_solution;
			}
		}
		temperature = settings.decreasing_function(temperature);
	}

	return min_solution;
}

#endif //PALLET_PACKING_CPP_ANNEALING_HPP

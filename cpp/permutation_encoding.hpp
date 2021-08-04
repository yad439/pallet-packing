#ifndef PALLET_PACKING_CPP_PERMUTATION_ENCODING_HPP
#define PALLET_PACKING_CPP_PERMUTATION_ENCODING_HPP

#include <utility>
#include <vector>
#include <tuple>
#include <random>

class PermutationEncoding {
	enum ChangeType { SWAP, MOVE };
public:
	explicit PermutationEncoding(std::vector<size_t> permutation);

	std::tuple<ChangeType, size_t, size_t> random_change(std::default_random_engine &rng);

	void restore(std::tuple<ChangeType, size_t, size_t> change);

	[[nodiscard]] const std::vector<size_t> &permutation() const;

private:
	std::tuple<ChangeType, size_t, size_t> generate_change(std::default_random_engine &rng);

	std::vector<size_t> _permutation;
	std::bernoulli_distribution _bool_distribution;
	std::uniform_int_distribution<size_t> _int_distribution;
};

#endif //PALLET_PACKING_CPP_PERMUTATION_ENCODING_HPP

#include "permutation_encoding.hpp"

using namespace std;

PermutationEncoding::PermutationEncoding(vector<size_t> permutation) : _permutation(std::move(permutation))
                                                                       , _int_distribution(0,
                                                                                           _permutation.size() - 1) {}

tuple<PermutationEncoding::ChangeType, size_t, size_t> PermutationEncoding::random_change(default_random_engine &rng) {
	auto change = generate_change(rng);

	if (get<0>(change) == SWAP)swap(_permutation[get<1>(change)], _permutation[get<2>(change)]);
	else {
		auto arg1 = get<1>(change);
		auto arg2 = get<2>(change);
		auto tmp = _permutation[arg1];
		_permutation.erase(_permutation.begin() + static_cast<ptrdiff_t>(arg1));
		_permutation.insert(_permutation.begin() + static_cast<ptrdiff_t>(arg2), tmp);
	}

	return change;
}

void PermutationEncoding::restore(tuple<ChangeType, size_t, size_t> change) {
	if (get<0>(change) == SWAP)swap(_permutation[get<1>(change)], _permutation[get<2>(change)]);
	else {
		auto arg1 = get<2>(change);
		auto arg2 = get<1>(change);
		auto tmp = _permutation[arg1];
		_permutation.erase(_permutation.begin() + static_cast<ptrdiff_t>(arg1));
		_permutation.insert(_permutation.begin() + static_cast<ptrdiff_t>(arg2), tmp);
	}
}

tuple<PermutationEncoding::ChangeType, size_t, size_t>
PermutationEncoding::generate_change(default_random_engine &rng) {
	while (true) {
		const auto type = _bool_distribution(rng) ? SWAP : MOVE;
		const auto arg1 = _int_distribution(rng);
		const auto arg2 = _int_distribution(rng);
		if (arg1 != arg2)return {type, arg1, arg2};
	}
}

const vector<size_t> &PermutationEncoding::permutation() const {
	return _permutation;
}

#include "skyline.hpp"

#include <list>
#include <functional>
#include <cassert>

using namespace std;

int skyline_decode(const int pallet_width, const int pallet_height, const span<const Item> items,
                   const span<const size_t> permutation, const span<Position> results) {
	list<pair<int, int>> skyline;
	skyline.emplace_back(0, 0);

	list<tuple<int, int, size_t>> items_list;
	for (auto i:permutation)items_list.emplace_back(items[i].width, items[i].height, i);

	auto packed = 0;
	while (!items_list.empty()) {
		auto gap = skyline.end();
		auto level = pallet_height;
		for (auto it = skyline.begin(); it != skyline.end(); ++it)
			if (it->second < level) {
				level = it->second;
				gap = it;
			}

		if (level == pallet_height)break;
		assert(gap != skyline.end());

		const auto next = ++gap;
		--gap;
		const auto is_not_begin = gap != skyline.begin();
		const auto prev = is_not_begin ? --gap : skyline.end();
		if (is_not_begin)++gap;

		const auto width = next != skyline.end() ? next->first - gap->first : pallet_width - gap->first;
		const auto height1 = prev != skyline.end() ? prev->second - gap->second : pallet_height - gap->second;
		const auto height2 = next != skyline.end() ? next->second - gap->second : pallet_height - gap->second;

		auto best_item = items_list.end();
		auto score = -1;
		bool rotated{};
		for (auto it = items_list.begin(); it != items_list.end(); ++it) {
			const auto current_item = *it;
			if (get<1>(current_item) <= pallet_height - gap->second && get<0>(current_item) <= width) {
				auto cur_score = static_cast<int>((get<1>(current_item) == height1))
				                 + static_cast<int>((get<1>(current_item) == height2))
				                 + static_cast<int>((get<0>(current_item) == width));
				assert(0 <= cur_score && cur_score <= 3);
				if (cur_score > score) {
					score = cur_score;
					best_item = it;
					rotated = false;
				}
			}
			if (get<0>(current_item) <= pallet_height - gap->second && get<1>(current_item) <= width) {
				auto cur_score = static_cast<int>((get<0>(current_item) == height1))
				                 + static_cast<int>((get<0>(current_item) == height2))
				                 + static_cast<int>((get<1>(current_item) == width));
				assert(0 <= cur_score && cur_score <= 3);
				if (cur_score > score) {
					score = cur_score;
					best_item = it;
					rotated = true;
				}
			}
		}
		const auto best_item_width = score != -1 ? (rotated ? get<1>(*best_item) : get<0>(*best_item)) : 0;
		const auto best_item_height = score != -1 ? (rotated ? get<0>(*best_item) : get<1>(*best_item)) : 0;
		const auto item_index = score != -1 ? get<2>(*best_item) : 0;
		if (score != -1)packed += items[item_index].width * items[item_index].height;

		switch (score) {
			case -1: {
				if (height1 <= height2) {
					if (gap != skyline.begin()) {
						skyline.erase(gap);
						if (prev != skyline.end() && next != skyline.end() && prev->second == next->second)
							skyline.erase(next);
					} else {
						gap->second += height2;
						if (next != skyline.end())skyline.erase(next);
					}
				} else {
					if (gap != skyline.begin()) {
						next->first = gap->first;
						skyline.erase(gap);
					} else {
						gap->second += height2;
						if (next != skyline.end())skyline.erase(next);
					}
				}
				break;
			}
			case 0: {
				if (!results.empty())
					results[item_index] = Position{true, static_cast<double>(gap->first),
					                               static_cast<double>(gap->second), rotated};

				skyline.emplace(next, gap->first + best_item_width, gap->second);
				gap->second += best_item_height;

				items_list.erase(best_item);
				break;
			}
			case 1: {
				if (best_item_height == height1) {
					if (!results.empty())
						results[item_index] = Position{true, static_cast<double>(gap->first),
						                               static_cast<double>(gap->second), rotated};
					gap->first += best_item_width;
				} else if (best_item_height == height2) {
					if (next != skyline.end()) {
						if (!results.empty())
							results[item_index] = Position{true, static_cast<double>(next->first - best_item_width),
							                               static_cast<double>(gap->second), rotated};
						next->first -= best_item_width;
					} else {
						if (!results.empty())
							results[item_index] = Position{true, static_cast<double>(pallet_width - best_item_width),
							                               static_cast<double>(gap->second), rotated};
						skyline.emplace_back(pallet_width - best_item_width, gap->second + best_item_height);
					}
				} else {
					assert(best_item_width == width);
					if (!results.empty())
						results[item_index] = Position{true, static_cast<double>(gap->first),
						                               static_cast<double>(gap->second), rotated};
					gap->second += best_item_height;
				}
				items_list.erase(best_item);
				break;
			}
			case 2: {
				if (!results.empty())
					results[item_index] = Position{true, static_cast<double>(gap->first),
					                               static_cast<double>(gap->second), rotated};
				if (best_item_height != height1) {
					if (next != skyline.end())skyline.erase(next);
					gap->second += best_item_height;
				} else if (best_item_height != height2)skyline.erase(gap);
				else {
					assert(best_item_width != width);
					gap->first += best_item_width;
				}

				items_list.erase(best_item);
				break;
			}
			case 3: {
				if (!results.empty())
					results[item_index] = Position{true, static_cast<double>(gap->first),
					                               static_cast<double>(gap->second), rotated};

				skyline.erase(gap);
				if (next != skyline.end())skyline.erase(next);

				items_list.erase(best_item);
				break;
			}
			default: assert(false);
				break;
		}
	}
	return pallet_width * pallet_height - packed;
}

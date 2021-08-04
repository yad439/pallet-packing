#ifndef PALLET_PACKING_CPP_COMMON_H
#define PALLET_PACKING_CPP_COMMON_H

struct Item {
	int width;
	int height;
};

struct Position {
	bool placed;
	double x;
	double y;
	bool rotated;
};

#endif //PALLET_PACKING_CPP_COMMON_H

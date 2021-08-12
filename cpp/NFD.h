#pragma once
#include<algorithm>
#include "o-tree.h"
#include "side_fucnctions.h"
using namespace std;

struct comp
{
	bool operator()(const Data* a, const Data* b) {
		return a->height > b->height;
	}
};

void NFD(int H, int W);
#define _CRT_SECURE_NO_WARNINGS
#include<cstdlib>
#include<time.h>
#include "simulated_annealing.h"
#include "NFD.h"
using namespace std;

//"C:\\solution\\packing0_3.txt"

int main() {
	srand(time(nullptr));
	int H;
	int W;
	string method;
	cin >> H >> W;
	cin >> method;
	if (method == "NFD") {
		NFD(H, W);
	}
	else if (method == "SA") {
		double r;
		int num_of_iters;
		cin >> r;
		cin >> num_of_iters;
		simulated_annealing(H, W, r, num_of_iters);
	}
	return 0;
}

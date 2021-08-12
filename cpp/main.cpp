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
	cin >> H >> W;
	//simulated_annealing(H, W, 0.91);
	NFD(H, W);
	system("pause");
	return 0;
}
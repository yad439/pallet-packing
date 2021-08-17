#include "simulated_annealing.h"

void simulated_annealing(int H, int W, double r, int num_of_iters) {
	clock_t start = clock();
	vector<Data*> rectangles;
	vector<point*> positions;
	pushrect(rectangles);
	string T = create_nfd_string(rectangles, H, W);
	string T0 = T;
	vector<Data*> r0 = rectangles;
	bool change = true;
	double t;
	int L = pow((rectangles.size() - 1), 2);
	int k = 0;
	int S, S_0, Sc;
	t = 20;
	S = solution(T0, r0);
	Sc = S;
	int iterations = 0;
	while (k < 10 && iterations <= num_of_iters) {
		for (int j = 0; j <= L; j++) {
			iterations++;
			if (change) {
				rectangles = r0;
				T = neighborhood(T0, rectangles);
			}
			else {
				T = neighborhood(T, rectangles);
			}
			S_0 = solution(T, rectangles);
			double D = S_0 - S;
			if (D > 0) {
				double p = exp(-(D / t));
				if (0.01 * (rand() % 101) < p) {
					S = S_0;
					change = false;
					T0 = T;
					r0 = rectangles;
				}
				else {
					change = true;
				}
			}
			else {
				S = S_0;
				change = false;
				T0 = T;
				r0 = rectangles;
			}
		}
		t = r * t;
		if (Sc == S) {
			k++;

		}
		else {
			Sc = S;
			k = 0;
		}
	}
	rectangles = r0;
	T = T0;
	S = solution(T, rectangles, positions, H, W);
	T0.clear();
	clock_t end = clock();
	double sec = (double)(end - start) / CLOCKS_PER_SEC;
	display(T, S, rectangles, positions, sec);
}


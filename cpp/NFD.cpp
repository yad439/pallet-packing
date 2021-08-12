#include "NFD.h"

void NFD(int H, int W) {
	clock_t start = clock();
	vector<Data*> rectangles;
	vector<point*> positions;
	pushrect(rectangles);
	int n = size(rectangles);
	string t;
	int w_level = 0;
	int h_level = 0;
	int i = 1;
	int count = 1;
	int pl = 0;
	bool exit = true;
	sort(rectangles.begin(), rectangles.end(), comp());
	w_level = rectangles[0]->width;
	h_level = rectangles[0]->height;
	t = '0';
	while (i < n) {
		if (i > 1) {
			while (i != n && h_level + rectangles[i]->height > H) {
				rectangles.erase(rectangles.cbegin() + i);
				n--;
			}
			if (i == n) {
				exit = false;
			}
			else {
				h_level += rectangles[i]->height;
			}
		}
		while (exit == true && i != n && w_level + rectangles[i]->width <= W) {
			count++;
			t += '0';
			rectangles[i]->place = pl;
			w_level += rectangles[i]->width;
			i++;
			pl++;
		}
		for (int j = 0; j < count; j++) {
			t += '1';
			pl++;
		}

		count = 0;
		w_level = 0;
	}
	int S = solution(t, rectangles, positions, H, W);
	clock_t end = clock();
	double sec = (double)(end - start) / CLOCKS_PER_SEC;
	display(t, S, rectangles, positions, sec);
}

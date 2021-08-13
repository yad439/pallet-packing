#include "o-tree.h"



step* newstep(int l, int r, int h) {
	step* st = new step(l, r, h);
	return st;
}

point* newpoint(int x, int y) {
	point* d = new point(x, y);
	return d;
}

void rotating(Data*& rect) {
	if (rect->rotate == 1) {
		int tmp = rect->height;
		rect->height = rect->width;
		rect->width = tmp;
	}
}

void changecontour(Data* p, vector<step*>& c, int s, bool flag) {
	if (size(c) == 0) {
		c.push_back(newstep(0, p->width, p->height));
	}

	else if (!flag || s - 1 == size(c)) {
		c.push_back(newstep(c[s - 2]->r, c[s - 2]->r + p->width, p->height));
	}

	else if (flag) {
		int j = s - 1;
		int max_h = c[j]->h;
		if (j == 0) {
			while (j < size(c) && p->width > c[j]->r) {

				if (j + 1 == size(c)) {
					if (c[j]->h > max_h) {
						max_h = c[j]->h;
					}
					j++;
				}
				else {
					j++;
					if (c[j]->h > max_h) max_h = c[j]->h;
				}
			}
			if (j == size(c))
				j--;
		}
		else {
			while (j < size(c) && p->width > c[j]->r - c[s - 1]->l) {
				if (j + 1 == size(c)) {
					if (c[j]->h > max_h)
						max_h = c[j]->h;
					j++;

				}
				else {
					j++;
					if (c[j]->h > max_h)
						max_h = c[j]->h;
				}

			}
			if (j == size(c))
				j--;
		}
		vector<step*> c_new;
		int k = 0;
		while (k < s - 1) {
			c_new.push_back(newstep(c[k]->l, c[k]->r, c[k]->h));
			k++;
		}
		c_new.push_back(newstep(c[s - 1]->l, p->width + c[s - 1]->l, p->height + max_h));
		if (p->width < c[j]->r - c[s - 1]->l) {
			if (j == 0)
				c_new.push_back(newstep(p->width, c[j]->r, c[j]->h));
			else
				c_new.push_back(newstep(c_new[size(c_new) - 1]->r, c[j]->r, c[j]->h));
			if (c_new[size(c_new) - 1]->l == c_new[size(c_new) - 1]->r)
				c_new.pop_back();
		}

		while (j + 1 < size(c)) {
			c_new.push_back(newstep(c[j + 1]->l, c[j + 1]->r, c[j + 1]->h));
			j++;
		}
		int d = size(c) - 1;
		int v = size(c_new) - 1;
		for (int z = 0; z <= d; z++) {
			delete c[z];
		}
		c.clear();
		for (int z = 0; z <= v; z++) {
			c.push_back(newstep(c_new[z]->l, c_new[z]->r, c_new[z]->h));
			delete c_new[z];
		}
		c_new.clear();
	}
}

vector<step*> round(const string& T, vector<Data*>& rectangles, vector<point*>& positions, bool last) {
	vector<step*> c;
	bool flag = false;
	int s = 0;
	int i = 0;
	int pr = 0;
	for (i = 0; i < size(T); i++) {
		if (T[i] == '0') {
			s++;
			rectangles[pr]->place = i;
			changecontour(rectangles[pr], c, s, flag);
			if (last) {
				positions.push_back(newpoint(c[s - 1]->l, c[s - 1]->h - rectangles[pr]->height));
			}
			pr++;
		}
		else
		{
			s--;
			flag = true;
		}
	}
	return c;
}

int solution(const string& T, vector<Data*>& rectangles) {
	vector<point*> positions;
	int maxH = 0;
	int maxW = 0;
	vector<step*> contour;
	contour = round(T, rectangles, positions, false);
	int v = size(contour);
	for (int i = 0; i < v; i++) {
		if (contour[i]->h >= maxH)
			maxH = contour[i]->h;
		if (contour[i]->r >= maxW)
			maxW = contour[i]->r;
		delete contour[i];
	}
	contour.clear();
	return maxH * maxW;
}

void remove_excess(string& T, vector<Data*>& rectangles, vector<point*>& positions, int H, int W) {
	vector<step*> c;
	int i = 0;
	while (i < size(positions)) {
		if (positions[i]->x + rectangles[i]->width > W || positions[i]->y + rectangles[i]->height > H) {
			int j = rectangles[i]->place;
			int s = 0;
			int k = 0;
			if (T.substr(rectangles[i]->place, 2) == "01") {
				T.erase(rectangles[i]->place, 2);
			}
			else {
				do {
					if (T[j + k] == '0') {
						s++;
						j++;
					}
					else {
						s--;
						k++;
					}
				} while (s != 0);
				int one = j + k - 1;
				T.erase(one, 1);
				T.erase(rectangles[i]->place, 1);
			}
			rectangles.erase(rectangles.cbegin() + i);
			positions.clear();
			c = round(T, rectangles, positions, true);
			c.clear();
		}
		else {
			i++;
		}
	}
}

int solution(string& T, vector<Data*>& rectangles, vector<point*>& positions, int H, int W) {
	int maxH = 0;
	int maxW = 0;
	vector<step*> contour;
	contour = round(T, rectangles, positions, true);
	remove_excess(T, rectangles, positions, H, W);
	positions.clear();
	contour = round(T, rectangles, positions, true);
	int v = size(contour);
	for (int i = 0; i < v; i++) {
		if (contour[i]->h >= maxH)
			maxH = contour[i]->h;
		if (contour[i]->r >= maxW)
			maxW = contour[i]->r;
		delete contour[i];
	}
	contour.clear();
	return maxH * maxW;
}

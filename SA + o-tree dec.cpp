#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include<time.h>
#include<math.h>
#include<fstream>
using namespace std;

struct Data {
	int width;
	int height;
	int place;
	int rotate;
};

vector<Data*> rectangles;

struct step {
	int l;
	int r;
	int h;
	step(int l, int r, int h) {
		this->l = l;
		this->r = r;
		this->h = h;
	}
};

struct dot {
	int x;
	int y;
	dot(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

vector<dot*> angles;

step* newstep(int l, int r, int h) {
	step* st = new step(l, r, h);
	return st;
}
dot* newdot(int x, int y) {
	dot* d = new dot(x, y);
	d->x = x;
	d->y = y;
	return d;
}
void rotating(Data* rect) {
	if (rect->rotate == 1) {
		int tmp = rect->height;
		rect->height = rect->width;
		rect->width = tmp;
	}
}
string create_simple_string(int n) {
	string t;
	for (int i = 0; i < n; i++) {
		t += '0';
	}
	for (int i = 0; i < n; i++) {
		t += '1';
	}
	return t;
}

string create_simple_string1(int n) {
	string t;
	for (int i = 0; i < n; i++) {
		t += "01";
	}
	return t;
}

void pushrect() {
	int w;
	int h;
	ifstream fin;
	fin.open("C:\\solution\\packing0_3.txt");
	while (fin >> w >> h) {
		int r = rand() % 2;
		Data* tmp = new Data;
		tmp->width = w;
		tmp->height = h;
		tmp->rotate = r;
		//rotating(tmp);
		rectangles.push_back(tmp);
	}
	fin.close();
}
string locality(string T) {
	int num = size(rectangles) - 1;
	int i = rand() % num;
	int j = rand() % num;
	int k = 0;
	if (T.substr(rectangles[i]->place, 2) == "01") {
		T.erase(rectangles[i]->place, 2);
		int k = rand() % (size(T) - 1);
		if (k == rectangles[i]->place - 1) {
			k = rand() % (size(T) - 1);
		}
		T.insert(k, "01");
		Data* tmp = new Data;
		int r = rand() % 2;
		tmp = rectangles[i];
		tmp->rotate = r;
		//rotating(tmp);
		rectangles.erase(rectangles.cbegin() + i);
		rectangles.insert(rectangles.cbegin() + j, tmp);
	}
	else {
		int r1 = rand() % 2;
		int r2 = rand() % 2;
		rectangles[j]->rotate = r1;
		rectangles[i]->rotate = r2;
		//rotating(rectangles[i]);
		//rotating(rectangles[j]);
		swap(rectangles[i], rectangles[j]);
	}
	return T;
}

vector<step*> changecontour(Data* p, vector<step*> c, int s, bool flag) {
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
	return c;
}

vector<step*> round(string T, bool a) {
	vector<step*> c;
	bool flag = false;
	int s = 0;
	int i = 0;
	double scale1 = 30;
	double scale2 = 30;
	int pr = 0;
	for (i = 0; i < size(T); i++) {
		if (T[i] == '0') {
			s++;
			rectangles[pr]->place = i;
			c = changecontour(rectangles[pr], c, s, flag);
			if (a) {
				angles.push_back(newdot(c[s - 1]->l, c[s - 1]->h - rectangles[pr]->height));
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
int solution(string T, int H, int W) {
	int S_pack = 0;
	int maxH = 0;
	int maxW = 0;
	vector<step*> contour;
	bool a = false;
	contour = round(T, a);
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
int solution_last(string T, int H, int W) {
	int S_pack = 0;
	int maxH = 0;
	int maxW = 0;
	vector<step*> contour;
	bool a = true;
	contour = round(T, a);
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


int simulated_annealing(string T, int H, int W, int r) {
	string T0 = T;
	vector<Data*> r0 = rectangles;
	bool change = true;
	double t;
	int L = pow((rectangles.size() - 1), 2);
	int k = 0;
	int S, S_0, Sc;
	t = 20;
	S = solution(T0, H, W);
	Sc = S;
	while (k < 10) {
		for (int j = 0; j <= L; j++) {
			if (change) {
				rectangles = r0;
				T = locality(T0);
			}
			else {
				T = locality(T);
			}
			S_0 = solution(T, H, W);
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
	S = solution_last(T0, H, W);
	int v = rectangles.size();
	cout << T0;
	T0.clear();
	return S;
}

int main() {
	srand(time(nullptr));
	clock_t start = clock();
	pushrect();
	string T = create_simple_string(size(rectangles) - 1);
	int S = simulated_annealing(T, 20, 20, 0.99);
	T.clear();
	cout << "\n";
	cout << "S:" << S << endl;
	cout << "packing:" << endl;
	for (int i = 0; i < rectangles.size(); i++) {
		cout << rectangles[i]->width << ", " << rectangles[i]->height << endl;
	}
	cout << "left bottom angle:" << endl;
	for (int i = 0; i < angles.size(); i++) {
		cout << '(' << angles[i]->x << ',' << angles[i]->y << ')' << endl;
	}
	clock_t end = clock();
	double sec = (double)(end - start) / CLOCKS_PER_SEC;
	cout << sec << " seconds" << endl;
	system("pause");
	return 0;
}
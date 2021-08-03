#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<fstream>
#include<algorithm>
#include<vector>
#include<string>
#include "o-tree.h" 
using namespace std;


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

void pushrect(vector<Data*>& rectangles)
{
	int w;
	int h;
	ifstream fin;
	fin.open("C:\\solution\\packing0_3.txt");
	while (fin >> w >> h) {
		int r = rand() % 2;
		Data* tmp = new Data(w, h, 0, r);
		rotating(tmp);
		rectangles.push_back(tmp);
	}
	fin.close();
}

struct comp
{
	bool operator()(const Data* a, const Data* b) {
		return a->height > b->height;
	}
};
string NFD(int W, int H, int n, vector<Data*>& rectangles) {
	string t;
	int h_level = 0;
	int w_level = 0;
	int i = 1;
	int count = 1;
	int pl = 0;
	sort(rectangles.begin(), rectangles.end(), comp());
	w_level = rectangles[0]->width;
	t = '0';
	while (i < n) {
		while (i != n && w_level + rectangles[i]->width <= W) {
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
	return t;
}



string neighborhood(string T, vector<Data*>& rectangles) {
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
		int r = rand() % 2;
		Data* tmp = new Data(rectangles[i]->width, rectangles[i]->height, rectangles[i]->place, r);
		rotating(tmp);
		rectangles.erase(rectangles.cbegin() + i);
		rectangles.insert(rectangles.cbegin() + j, tmp);
	}
	else {
		int r1 = rand() % 2;
		int r2 = rand() % 2;
		rectangles[j]->rotate = r1;
		rectangles[i]->rotate = r2;
		swap(rectangles[i], rectangles[j]);
	}
	return T;
}

int simulated_annealing(string& T, vector<Data*>& rectangles, vector<point*>& positions, int H, int W, double r) {
	string T0 = T;
	vector<Data*> r0 = rectangles;
	bool change = true;
	double t;
	int L = pow((rectangles.size() - 1), 2);
	int k = 0;
	int S, S_0, Sc;
	t = 20;
	S = solution(T0, r0, H, W);
	Sc = S;
	while (k < 10) {
		for (int j = 0; j <= L; j++) {
			if (change) {
				rectangles = r0;
				T = neighborhood(T0, rectangles);
			}
			else {
				T = neighborhood(T, rectangles);
			}
			S_0 = solution(T, rectangles, H, W);
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
	int v = rectangles.size();
	T0.clear();
	return S;
}

int main() {
	srand(time(nullptr));
	clock_t start = clock();
	vector<Data*> rectangles;
	vector<point*> positions;
	pushrect(rectangles);
	int H = 50;
	int W = 50;
	//string T = create_simple_string1(size(rectangles));
	//int S = simulated_annealing(T, rectangles, positions, H, W, 0.99);
	string T = NFD(H, W, size(rectangles), rectangles);
	int S = solution(T, rectangles, positions, H, W);
	cout << T;
	T.clear();
	cout << "\n";
	cout << "S:" << S << endl;
	cout << "packing:" << endl;
	for (int i = 0; i < rectangles.size(); i++) {
		cout << rectangles[i]->width << ", " << rectangles[i]->height << endl;
	}
	cout << "left bottom angle:" << endl;
	for (int i = 0; i < positions.size(); i++) {
		cout << '(' << positions[i]->x << ',' << positions[i]->y << ')' << endl;
	}
	clock_t end = clock();
	double sec = (double)(end - start) / CLOCKS_PER_SEC;
	cout << sec << " seconds" << endl;
	system("pause");
	return 0;
}
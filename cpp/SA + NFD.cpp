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

void display(const string& T, int S, const vector<Data*>& rectangles, const vector<point*>& positions, double sec) {
	cout << T << endl;
	cout << "S:" << S << endl;
	cout << "packing:" << endl;
	for (int i = 0; i < rectangles.size(); i++) {
		cout << rectangles[i]->width << ", " << rectangles[i]->height << endl;
	}
	cout << "left bottom angle:" << endl;
	for (int i = 0; i < positions.size(); i++) {
		cout << '(' << positions[i]->x << ',' << positions[i]->y << ')' << endl;
	}
	cout << sec << " seconds" << endl;
}

void pushrect(vector<Data*>& rectangles)
{
	int w;
	int h;
	ifstream fin;
	string way;
	cin >> way;
	fin.open(way);
	while (fin >> w >> h) {
		int r = rand() % 2;
		Data* tmp = new Data(w, h, 0, r);
		rotating(tmp);
		rectangles.push_back(tmp);
	}
	fin.close();
}
//"C:\\solution\\packing0_3.txt"
struct comp
{
	bool operator()(const Data* a, const Data* b) {
		return a->height > b->height;
	}
};
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

void simulated_annealing(int H, int W, double r) {
	clock_t start = clock();
	vector<Data*> rectangles;
	vector<point*> positions;
	pushrect(rectangles);
	string T = create_simple_string1(size(rectangles));
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
	T0.clear();
	clock_t end = clock();
	double sec = (double)(end - start) / CLOCKS_PER_SEC;
	display(T, S, rectangles, positions, sec);
}

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
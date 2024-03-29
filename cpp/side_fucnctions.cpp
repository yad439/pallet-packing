#include "side_fucnctions.h"

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

string create_nfd_string(vector<Data*> &rectangles, int H, int W){
	int n = size(rectangles);
	string t;
	int w_level = 0;
	int i = 1;
	int count = 1;
	sort(rectangles.begin(), rectangles.end(), comp());
	w_level = rectangles[0]->width;
	t = '0';
	while (i < n) {
		while (i != n && w_level + rectangles[i]->width <= W) {
			count++;
			t += '0';
			w_level += rectangles[i]->width;
			i++;
		}
		for (int j = 0; j < count; j++) {
			t += '1';
		}

		count = 0;
		w_level = 0;
	}
	return t;
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

void display(const string& T, int S, const vector<Data*>& rectangles, const vector<point*>& positions, double sec) {
	for (int i = 0; i < rectangles.size(); i++) {
		cout << rectangles[i]->width << " " << rectangles[i]->height << endl;
	}
	cout << "|" << endl;
	for (int i = 0; i < positions.size(); i++) {
		cout << positions[i]->x << " " << positions[i]->y << endl;
	}
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


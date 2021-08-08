#pragma once
#include<vector>
#include<string>
using namespace std;

struct Data {
	int width;
	int height;
	int place;
	int rotate;
	Data(int w, int h, int p, int r) {
		this->width = w;
		this->height = h;
		this->place = p;
		this->rotate = r;
	}
};
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
struct point {
	int x;
	int y;
	point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

step* newstep(int l, int r, int h);

point* newpoint(int x, int y);

void rotating(Data*& rect);

void changecontour(Data* p, vector<step*>& c, int s, bool flag);

vector<step*> round(const string& T, vector<Data*>& rectangles, vector<point*>& positions, bool last);

int solution(const string& T, vector<Data*>& rectangles, int H, int W);

void remove_excess(string& T, vector<Data*>& rectangles, vector<point*>& positions, int H, int W);

int solution(string& T, vector<Data*>& rectangles, vector<point*>& positions, int H, int W);
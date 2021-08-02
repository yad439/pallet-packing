#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
#include<glut.h>
#include<time.h>
#include<math.h>
#include<algorithm>
#include<vector>
#include<fstream>
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

step* newstep(int l, int r, int h) {
	step* st = new step(l, r, h);
	return st;
}
point* newpoint(int x, int y) {
	point* d = new point(x, y);
	return d;
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

void rotating(Data* &rect) {
	if (rect->rotate == 1) {
		int tmp = rect->height;
		rect->height = rect->width;
		rect->width = tmp;
	}
}

void pushrect(vector<Data*> &rectangles)
{
	int w;
	int h;
	ifstream fin;
	fin.open("C:\\solution\\packing0.4.txt");
	while (fin >> w >> h) {
		int r = rand() % 2;
		Data* tmp = new Data(w, h, 0, r);
		//rotating(tmp);
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
string NFD(int W, int H, int n, vector<Data*> &rectangles) {
	string t;
	int w_level = 0;
	int i = 1;
	int count = 1;
	int pl = 0;
	sort(rectangles.begin(), rectangles.end(), comp());
	w_level = rectangles[0]->width;
	t = '0';
	while(i < n) { 
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

void rect(int x1, int x2, int y1, int y2, double scale1, double scale2) {
	glColor3d(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2d(double(x1) / scale1, (double(y2) - double(y1)) / scale2);
	glVertex2d(double(x2) / scale1, (double(y2) - double(y1)) / scale2);
	glVertex2d(double(x2) / scale1, double(y2) / scale2);
	glVertex2d(double(x1) / scale1, double(y2) / scale2);
	glEnd();
	glColor3d(0, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2d(double(x1) / scale1, (double(y2) - double(y1)) / scale2);
	glVertex2d(double(x2) / scale1, (double(y2) - double(y1)) / scale2);
	glVertex2d(double(x2) / scale1, double(y2) / scale2);
	glVertex2d(double(x1) / scale1, double(y2) / scale2);
	glEnd();
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
	double scale1 = 30;
	double scale2 = 30;
	int pr = 0;
	for (i = 0; i < size(T); i++) {
		if (T[i] == '0') {
			s++;
			rectangles[pr]->place = i;
			changecontour(rectangles[pr], c, s, flag);
			rect(c[s - 1]->l, c[s - 1]->r, rectangles[pr]->height, c[s - 1]->h, scale1, scale2);
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

int solution(const string& T, vector<Data*>& rectangles, int H, int W) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vector<point*> positions;
	int maxH = 0;
	int maxW = 0;
	vector<step*> contour;
	contour = round(T, rectangles, positions, false);
	glColor3d(1, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	double scale = 30;
	glVertex2d(0, 0);
	glVertex2d(0, double(H) / scale);
	glVertex2d(double(W) / scale, double(H) / scale);
	glVertex2d(double(W) / scale, 0);
	glEnd();
	int v = size(contour);
	for (int i = 0; i < v; i++) {
		if (contour[i]->h >= maxH)
			maxH = contour[i]->h;
		if (contour[i]->r >= maxW)
			maxW = contour[i]->r;
		delete contour[i];
	}
	contour.clear();
	glutSwapBuffers();
	return maxH * maxW;
}

void remove_excess(string &T, vector<Data*> &rectangles, vector<point*> &positions, int H, int W) {
	int count = 0;
	for (int i = 0; i < size(positions); i++) {
		if (positions[i]->x + rectangles[i]->width > W || positions[i]->y + rectangles[i]->height > H) {
			int k = T.find_first_not_of('0', rectangles[i]->place - count);
			T.erase(k, 1);
			T.erase(rectangles[i]->place - count, 1);
			rectangles.erase(rectangles.cbegin() + i);
			positions.erase(positions.cbegin() + i);
			count += 2;
		}
	}
}

int solution(string& T, vector<Data*>& rectangles, vector<point*>& positions, int H, int W) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int maxH = 0;
	int maxW = 0;
	vector<step*> contour;
	contour = round(T, rectangles, positions, true);
	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	remove_excess(T, rectangles, positions, H, W);
	positions.clear();
	contour = round(T, rectangles, positions, true);
	glColor3d(1, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	double scale = 30;
	glVertex2d(0, 0);
	glVertex2d(0, double(H) / scale);
	glVertex2d(double(W) / scale, double(H) / scale);
	glVertex2d(double(W) / scale, 0);
	glEnd();
	int v = size(contour);
	for (int i = 0; i < v; i++) {
		if (contour[i]->h >= maxH)
			maxH = contour[i]->h;
		if (contour[i]->r >= maxW)
			maxW = contour[i]->r;
		delete contour[i];
	}
	contour.clear();
	glutSwapBuffers();
	return maxH * maxW;
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
		//k = T.find_first_not_of('0', k);
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

int simulated_annealing(string &T, vector<Data*> &rectangles, vector<point*> &positions, int H, int W, double r) {
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
	return S;
}
void draw() {
	clock_t start = clock();
	vector<Data*> rectangles;
	vector<point*> positions;
	pushrect(rectangles);
	int H = 10;
	int W = 10;
	//string T = create_simple_string1(size(rectangles));
	//int S = simulated_annealing(T, rectangles, positions, H, W, 0.91);
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
}
int main(int argc, char** argv) {
	srand(time(nullptr));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(400, 0);
	glutCreateWindow("Packing");
	glTranslated(-1, -1, 0);
	glutDisplayFunc(draw);
	glutMainLoop();
	system("pause");
	return 0;
}
/*
12 - 252 площадь и 8 секунд, 5% пустоты      0.2
15 - 286 площадь и 16 секунд, 6.5% пустоты   0.1
20 - 3360 площадь и 30 секунд,  2% пустоты   0.3
что делать если выпирают какие-то прямоугольники, куда девать?
медленно считает
вообще в принципе как сделать так, чтобы эти функции можно было вызывать
*/
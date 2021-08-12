#pragma once
#include<iostream>
#include<fstream>
#include "o-tree.h"
using namespace std;
string create_simple_string(int n);
string create_simple_string1(int n);
void pushrect(vector<Data*>& rectangles);
void display(const string& T, int S, const vector<Data*>& rectangles, const vector<point*>& positions, double sec);
string neighborhood(string T, vector<Data*>& rectangles);
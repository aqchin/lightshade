#pragma once

#include <stdio.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "Vector3.h"

using namespace std;

class objLoader {
public:
	bool isLoaded;
	vector<Vector3> v, rgb, vn;
	vector<int> v_ind[3], vn_ind[3]; // x0, y1, z2
	Vector3 max, min;
	objLoader();
	void parse(const char*);
};
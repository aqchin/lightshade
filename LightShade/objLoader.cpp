#define _CRT_SECURE_NO_DEPRECATE
#include "objLoader.h"

using namespace std;

const int static MAX_CHAR = 80;

objLoader::objLoader() {
	this->isLoaded = false;
	this->max = *new Vector3();
	this->min = *new Vector3(10,10,10);

	// Fill index [0]; '.obj' indicies start at [1]
	v.push_back(*new Vector3());
	vn.push_back(*new Vector3());
	rgb.push_back(*new Vector3());
}

void objLoader::parse(const char* obj) {
	FILE *f = fopen(obj, "r"); // Read the .obj file
	char line[MAX_CHAR + 1];

	// Bad file
	if (f == NULL) {
		cerr << "Error reading: " << obj << ". Bad file." << endl;
		return;
	}

	// Parse '.obj'
	while (fgets(line, MAX_CHAR, f)) {
		if (line[0] == 'v') {
			if (line[1] == 'n') { // Vector Normal
				float x, y, z;
				sscanf(&line[2], "%f %f %f\n", &x, &y, &z);
				float l = sqrt(x*x + y*y + z*z); // Magnitude
				vn.push_back(*new Vector3(x / l, y / l, z / l)); // Normalize
			}
			else if (line[1] == ' ') { // Vector
				float x, y, z, r, g, b;
				sscanf(&line[1], "%f %f %f %f %f %f\n", &x, &y, &z, &r, &g, &b);
				v.push_back(*new Vector3(x, y, z));
				rgb.push_back(*new Vector3(r, g, b));

				if (x < min.x) min.x = x;
				else if (x > max.x) max.x = x;
				if (y < min.y) min.y = y;
				else if (y > max.y) max.y = y;
				if (z < min.z) min.z = z;
				else if (z > max.z) max.z = z;
			}
			else {
				cerr << "Error reading: " << obj << ". Bad line." << endl;
				return;
			}
		}
		else if (line[0] == 'f' && line[1] == ' ') { // Face
			int x, xn, y, yn, z, zn;
			sscanf(&line[1], "%d//%d %d//%d %d//%d\n", &x, &xn, &y, &yn, &z, &zn);
			v_ind[0].push_back(x);
			v_ind[1].push_back(y);
			v_ind[2].push_back(z);
			vn_ind[0].push_back(xn);
			vn_ind[1].push_back(yn);
			vn_ind[2].push_back(zn);
		}
	}

	this->isLoaded = true; // Done parsing
	cout << "Done parsing '" << obj << "'. " << v_ind[0].size() << " faces. ";
	cout << v.size() - 1 << " vertices." << endl;
}
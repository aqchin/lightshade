#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <thread>

#include "Windows.h"
#include "GL/GLee.h"
#include "shader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Matrix4.h"
#include "objLoader.h"
#include "Vector3.h"
#include "Vector4.h"

#include <GL/glut.h>

namespace Globals
{
	extern Matrix4 m2w;
};

#endif
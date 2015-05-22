#pragma once

#include "Matrix4.h"

#include <GL/glut.h>

class Material {
private:
	void set(GLenum, GLfloat*);

public:
	GLenum face;

	Material();
	Material(GLenum);

	void setAmbient(GLfloat*);
	void setDiffuse(GLfloat*);
	void setSpecular(GLfloat*);
	void setEmission(GLfloat*);
	void setShine(GLfloat*);
};
#include "Material.h"

Material::Material() { this->face = GL_FRONT_AND_BACK; }
Material::Material(GLenum g) { this->face = g; }

void Material::set(GLenum g, GLfloat* f) { glMaterialfv(this->face, g, f); }

void Material::setAmbient(GLfloat* f) { set(GL_AMBIENT, f); }
void Material::setDiffuse(GLfloat* f) { set(GL_DIFFUSE, f); }
void Material::setSpecular(GLfloat* f) { set(GL_SPECULAR, f); }
void Material::setEmission(GLfloat* f) { set(GL_EMISSION, f); }
void Material::setShine(GLfloat* f) { set(GL_SHININESS, f); }
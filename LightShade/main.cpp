#include "main.h"

using namespace std;

static int width = 512, height = 512;
static double rotato = 0.01;
static double ROT_SCALE = 120.2;
static bool shade = false;
Matrix4 *m = new Matrix4(); // Temp Matrix4 used for transformations
Matrix4 ml1 = *new Matrix4();
Matrix4 ml2 = *new Matrix4();
Matrix4 ml = *new Matrix4();

int mode = 0;
objLoader *bunny, *dragon, *bear;
static int vind, x_prev, y_prev;
static bool rot = false;
static bool zom = false;
Vector3 v_prev = *new Vector3();
Vector3 v_cur = *new Vector3();
Vector3 v = *new Vector3();
Vector3 tmp = *new Vector3();
Vector3 v_rot = *new Vector3();

Vector3 *v_tmp = new Vector3();
Vector3 *vn_tmp = new Vector3();

Material mat = Material(GL_FRONT);
Light l1 = Light(1);
Light l2 = Light(2);

GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.0, 1.0 };
GLfloat mat_ambient_c[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat no_emission[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat mat_emission[] = { 0.1, 0.1, 0.1, 0.0 };

GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_specular[] = { 0.3, 0.3, 0.1, 1.0 };
GLfloat light_position[] = { 10.0, 10.0, 0.0, 0.0 };

GLfloat spot_ambient[] = { 0.1, 0.1, 0.0, 1.0 };
GLfloat spot_diffuse[] = { 0.9, 0.9, 0.0, 1.0 };
GLfloat spot_dir[] = { 0, 0, -1 };
GLfloat spot_pos[] = { -7.0, 0.0, 3.0, 1.0 };
GLfloat spot_co = 32.0;
GLfloat spot_exp = 0.0;

using namespace std;

namespace Globals
{
	Matrix4 m2w;
};

void spin(double d) {
	m->makeRotateY(d);
	Globals::m2w = Globals::m2w * (*m);
}

void draw(objLoader* obj) {
	glBegin(GL_TRIANGLES);
	glColor3d(1.0, 1.0, 1.0);
	int k = obj->v_ind[0].size();
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < 3; j++) {
			vind = (obj->v_ind[j][i]);
			*v_tmp = obj->v[vind];
			*vn_tmp = obj->vn[obj->vn_ind[j][i]];

			if (mode == 0) // Dragon and Bear are black by default
				glColor3d(obj->rgb[vind].x, obj->rgb[vind].y, obj->rgb[vind].z);
			glNormal3d(vn_tmp->x, vn_tmp->y, vn_tmp->z);
			glVertex3d(v_tmp->x, v_tmp->y, v_tmp->z);
		}
	}
	glEnd();
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

	// Smooth
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	*/

	// Tell OpenGL what ModelView matrix to use:
	Matrix4 glmatrix = Globals::m2w;

	//if(!shade) glEnable(GL_LIGHT0);
	//if (mode == 0) {
		glDisable(GL_LIGHTING);

		if (!shade) {
			glLoadMatrixd(ml1.getPointer());
			glColor3f(0.0, 0.0, 1.0);
			glutSolidSphere(0.1, 10, 10);
		}


		if (shade) mat.setAmbient(mat_ambient_c);
		glLoadMatrixd(ml2.getPointer());
		glColor3f(0.0, 1.0, 0.0);
		glutSolidCone(0.1, 0.2, 10, 10);
		//glEnable(GL_LIGHTING);


		glLoadMatrixd(ml.getPointer());

		//if (!shade) {
			// Light1
			l1.setAmbient(light_ambient);
			l1.setDiffuse(light_diffuse);
			l1.setSpecular(light_specular);
			l1.setPosition(light_position);
			l1.on();
		//}

		// Light2
		l2.setAmbient(spot_ambient);
		l2.setDiffuse(spot_diffuse);
		l2.setPosition(spot_pos);
		l2.setSpotCutoff(spot_co);
		l2.setSpotDirection(spot_dir);
		l2.setSpotExponent(spot_exp);
		l2.on(); 

	//glDisable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//if (shade) glDisable(GL_LIGHT0);

	glLoadMatrixd(glmatrix.getPointer());

	switch (mode) {
		case 0: // Bunny
			mat.setAmbient(mat_ambient);
			mat.setDiffuse(mat_diffuse);
			mat.setEmission(no_emission);
			mat.setShine(high_shininess);
			mat.setSpecular(mat_specular);
			draw(bunny);
			break;
		case 1: // Dragon
			mat.setAmbient(no_mat);
			mat.setDiffuse(mat_diffuse);
			mat.setEmission(mat_emission);
			mat.setShine(low_shininess);
			mat.setSpecular(mat_specular);
			draw(dragon);
			break;
		case 2: // Bear
			mat.setAmbient(mat_ambient_color);
			mat.setDiffuse(mat_diffuse);
			mat.setEmission(mat_emission);
			mat.setShine(no_shininess);
			mat.setSpecular(mat_specular);
			draw(bear);
			break;
	}

	glFlush();
	glutSwapBuffers();
}

void fps() {
	clock_t begin = clock();

	displayCallback();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	//cout << "FPS: " << 1.0 / elapsed_secs << endl;
	cout << "Draw Time: " << elapsed_secs << "s." << endl;
}

void reshapeCallback(int w, int h) {
	cerr << "reshapeCallback called." << endl;
	width = w;
	height = h;
	glViewport(0, 0, w, h);  // set new viewport size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0); // set perspective projection viewing frustum
	glTranslatef(0, 0, -20);    // move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glMatrixMode(GL_MODELVIEW);
}

Vector3 trackBallMap(int x, int y) {
	v.x = (2.0*x - width) / width;
	v.y = (height - 2.0*y) / height;
	v.z = 0.0;
	double d = v.length();
	//cout << d << endl;
	d = (d<1.0) ? d : 1.0;
	v.z = sqrt(1.001 - d*d);
	v.normalize();

	return v;
}

void processMouseKeys(int k, int s, int x, int y) {
	switch (k) {
		case (GLUT_LEFT_BUTTON) :
			rot = (s == GLUT_DOWN);
			v_prev = trackBallMap(x, y);
			break;
		case (GLUT_RIGHT_BUTTON) :
			zom = (s == GLUT_DOWN);
			v_prev = trackBallMap(x, y);
			x_prev = x;
			y_prev = y;
			break;
	}
}

void processMouseMovement(int x, int y) {
	//cout << x - x_prev << " " << y - y_prev << endl;
	v_cur = trackBallMap(x, y);

	if (rot) {
		tmp = (v_cur - v_prev);
		double vel = tmp.length();
		v_rot = v_rot.cross(v_prev, v_cur);
		double rot_ang = vel * ROT_SCALE;
		if(v_rot.length() != 0.0) v_rot.normalize();
		m->makeRotate(rot_ang, v_rot);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;
	}

	if (zom) {
		if (y < y_prev) {
			m->makeScale(1.05, 1.05, 1.05);
			Globals::m2w = (*m) * Globals::m2w;
		}
		else if (y > y_prev) {
			m->makeScale(0.95, 0.95, 0.95);
			Globals::m2w = (*m) * Globals::m2w;
		}
	}

	v_prev = v_cur;
	x_prev = x;
	y_prev = y;
	fps();
}

void processSpecialKeys(int k, int, int) {
	double x, y, z, s;
	switch (k) {
		case GLUT_KEY_F1: // Bunny
			Globals::m2w.identity();
			s = 30 * tan(M_PI / 6) / (bunny->max.x - bunny->min.x);
			x = -(bunny->max.x + bunny->min.x) / 2;
			y = -(bunny->max.y + bunny->min.y) / 2;
			z = -(bunny->max.z + bunny->min.z) / 2;
			m->makeTranslate(x, y, z);
			m->transpose();
			Globals::m2w = (*m) * Globals::m2w;
			m->makeScale(s, s, s);
			m->transpose();
			Globals::m2w = (*m) * Globals::m2w;
			mode = 0;
			break;
		case GLUT_KEY_F2: // Dragon
			if (!dragon->isLoaded) cout << "Dragon not parsed." << endl;
			else {
				Globals::m2w.identity();
				s = 40 * tan(M_PI / 6) / (dragon->max.x - dragon->min.x);
				x = -(dragon->max.x + dragon->min.x) / 2;
				y = -(dragon->max.y + dragon->min.y) / 2;
				z = -(dragon->max.z + dragon->min.z) / 2;
				m->makeTranslate(x, y, z);
				m->transpose();
				Globals::m2w = (*m) * Globals::m2w;
				m->makeScale(s, s, s);
				m->transpose();
				Globals::m2w = (*m) * Globals::m2w;
				mode = 1;
			}
			break;
		case GLUT_KEY_F3: // Bear
			if (!bear->isLoaded) cout << "Bear not parsed." << endl;
			else {
				Globals::m2w.identity();
				s = 40 * tan(M_PI / 6) / (bear->max.y - bear->min.y);
				x = -(bear->max.x + bear->min.x) / 2;
				y = -(bear->max.y + bear->min.y) / 2;
				z = -(bear->max.z + bear->min.z) / 2;
				m->makeTranslate(x, y, z);
				m->transpose();
				Globals::m2w = (*m) * Globals::m2w;
				m->makeScale(s, s, s);
				m->transpose();
				Globals::m2w = (*m) * Globals::m2w;
				mode = 2;
			}
			break;
	}
	fps();
}

void processNormalKeys(unsigned char key, int, int)
{
	switch (key){
	case 'p':
		shade = !shade;
		//if (shade) shader->bind();
		//else shader->unbind();
		if (shade) Shader shader = Shader("spotlight_shading.vert", "spotlight_shading.frag", true);
		else Shader shader = Shader("spotlight_shading.vert", "spotlight_shading.frag", false);
		cout << "Shader is: " << ((shade) ? "on" : "off") << endl;
		break;
	}
	/*
	switch (key) {
	case 'r':
		Globals::m2w.identity();
		break;
	case 'x':
		m->makeTranslate(-1, 0, 0);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;
		break;
	case 'X':
		m->makeTranslate(1, 0, 0);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;
		break;
	case 'y':
		m->makeTranslate(0, -1, 0);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;
		break;
	case 'Y':
		m->makeTranslate(0, 1, 0);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;
		break;
	case 'z':
		m->makeTranslate(0, 0, -1);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;
		break;
	case 'Z':
		m->makeTranslate(0, 0, 1);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;
		break;
	case 's':
		m->makeScale(0.9, 0.9, 0.9);
		Globals::m2w = Globals::m2w * (*m);
		break;
	case 'S':
		m->makeScale(1.1, 1.1, 1.1);
		Globals::m2w = Globals::m2w * (*m);
		break;
	case ',':
		m->makeRotateY(10);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;;
		break;
	case '<':
		m->makeRotateY(-10);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;;
		break;
	case '.':
		m->makeRotateX(10);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;;
		break;
	case '>':
		m->makeRotateX(-10);
		m->transpose();
		Globals::m2w = (*m) * Globals::m2w;;
		break;
	}
	*/
	fps();
}

void idleCallback() {
	//spin(rotato);
	fps();
}

void load() {
	bunny = new objLoader();
	dragon = new objLoader();
	bear = new objLoader();

	thread bunnyThread(&objLoader::parse, bunny, "bunny.obj");
	thread dragonThread(&objLoader::parse, dragon, "dragon.obj");
	thread bearThread(&objLoader::parse, bear, "bear.obj");

	bunnyThread.join();
	dragonThread.detach();
	bearThread.detach();

	return;
}

int main(int argc, char *argv[])
{
  //float specular[]  = {1.0, 1.0, 1.0, 1.0};
  //float shininess[] = {100.0};
  float position[]  = {0.0, 10.0, 1.0, 0.0};  // lightsource position
  
  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(width, height);		  // set initial window size
  glutCreateWindow("OpenGL;HF LIGHT & SHADOW");  // open window and set window title

  glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
  glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
  glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
  //glDisable(GL_CULL_FACE);				  // disable backface culling to render both sides of polygons
  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);             	      // set shading to smooth
  glMatrixMode(GL_PROJECTION); 
  
  // Generate material properties:
  //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glEnable(GL_COLOR_MATERIAL);
  
  // Generate light source:
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Lighting thing
  glEnable(GL_NORMALIZE);
  //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  
  // Install callback functions:
  glutDisplayFunc(displayCallback); // Change to fps to show fps
  glutReshapeFunc(reshapeCallback);
  //glutIdleFunc(idleCallback);

  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  glutMouseFunc(processMouseKeys);
  glutMotionFunc(processMouseMovement);

  // Initialize cube matrix:
  Globals::m2w = (*new Matrix4());
  Globals::m2w.identity();

  load();

  double s = 30 * tan(M_PI / 6) / (bunny->max.x - bunny->min.x);
  m->makeScale(s, s, s);
  m->transpose();
  Globals::m2w = (*m) * Globals::m2w;

  m->makeTranslate(light_position[0], light_position[1], light_position[2]);
  m->transpose();
  ml1 = (*m) * Globals::m2w;

  m->makeRotateY(270);
  m->transpose();
  ml2 = (*m) * Globals::m2w;
  m->makeTranslate(spot_pos[0], spot_pos[1], spot_pos[2]);
  m->transpose();
  ml2 = (*m) * ml2;

  ml.identity();
    
  glutMainLoop();
  return 0;
}
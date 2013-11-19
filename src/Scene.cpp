#include <fstream>
#include "Scene.h"
#include "GL/glut.h"
#include "InvalidPreAttrException.h"
#include "InvalidTexFile.h"
#include <map>
#include "MyTorus.h"
#include "CGFcamera.h"
#include <iostream>
#include <sstream>
#include <string.h>

using namespace std;

Scene *Scene::instance = NULL;
unsigned int Scene::HEIGHT = 768;
unsigned int Scene::WIDTH = 1024;

extern int main_window;
float view_rotate[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

float obj_pos[] = { 0.0, 0.0, 0.0 };

Scene::Scene() {
	bckg_r = 0;
	bckg_g = 0;
	bckg_b = 0;
	bckg_a = 0;
	drawmode = GL_FILL;
	shading = GL_SMOOTH;
	cullface = GL_CCW;
	cullorder = GL_BACK;
	doublesided = true;
	local = true;
	enabled = true;
	amb_r = 0;
	amb_g = 0;
	amb_b = 0;
	amb_a = 0;

	appearances.insert(AppearanceElem::value_type("default", new Appearance()));
}

void Scene::setBackground(float bckg_r, float bckg_g, float bckg_b, float bckg_a) {
	this->bckg_r = bckg_r;
	this->bckg_g = bckg_g;
	this->bckg_b = bckg_b;
	this->bckg_a = bckg_a;
}

void Scene::setDrawmode(string drawmode) {
	if (drawmode == "fill") {
		this->drawmode = GL_FILL;
	} else if (drawmode == "line") {
		this->drawmode = GL_LINE;
	} else if (drawmode == "point") {
		this->drawmode = GL_POINT;
	} else {
		throw InvalidPreAttrException("drawmode");
	}
}

void Scene::setShading(string shading) {
	if (shading == "flat") {
		this->shading = GL_FLAT;
	} else if (shading == "gouraud") {
		this->shading = GL_SMOOTH;
	} else {
		throw InvalidPreAttrException("shading");
	}
}

void Scene::setCullface(string cullface) {
	if (cullface == "none") {
		this->cullface = GL_NONE;
	} else if (cullface == "back") {
		this->cullface = GL_BACK;
	} else if (cullface == "front") {
		this->cullface = GL_FRONT;
	} else if (cullface == "both") {
		this->cullface = GL_FRONT_AND_BACK;
	} else {
		throw InvalidPreAttrException("cullface");
	}
}

void Scene::setCullorder(string cullorder) {
	if (cullorder == "CCW") {
		this->cullorder = GL_CCW;
	} else if (cullorder == "CW") {
		this->cullorder = GL_CW;
	} else {
		throw InvalidPreAttrException("cullorder");
	}
}

void Scene::setRootId(string rootId) {
	this->rootId = rootId;
}

void Scene::setInitCamera(string init_camera) {
	this->init_camera = init_camera;
}

void Scene::addLight(Light* light) {
	lights.push_back(light);
}

void Scene::addCamera(string key, Camera* camera) {
	cameras.insert(CameraElem::value_type(key, camera));
}

void Scene::addTexture(string key, string path) {
	ifstream file;
	file.open(path.c_str(), ios::out);
	if (file.is_open()) {
		CGFtexture *tex = new CGFtexture(path);
		textures.insert(TexElem::value_type(key, tex));
		file.close();
	} else {
		throw InvalidTexFile(path);
	}

}

void Scene::addAppearance(string key, Appearance* appearance) {
	appearances.insert(AppearanceElem::value_type(key, appearance));
}

void Scene::addAnimation(string key, Animation* animation) {
	animations.insert(AnimationElem::value_type(key, animation));
}

void Scene::addNode(string key, Node* node) {
	graph.insert(GraphElem::value_type(key, node));
}

void Scene::setGlobalLights(bool doublesided, bool local, bool enabled) {
	this->doublesided = doublesided;
	this->local = local;
	this->enabled = enabled;
}

void Scene::setAmb(float amb_r, float amb_g, float amb_b, float amb_a) {
	this->amb_r = amb_r;
	this->amb_g = amb_g;
	this->amb_b = amb_b;
	this->amb_a = amb_a;
}

unsigned int Scene::getDrawmode() {
	return drawmode;
}

unsigned int Scene::getShading() {
	return shading;
}

const unsigned int Scene::getCullface() {
	return cullface;
}

const unsigned int Scene::getCullorder() {
	return cullorder;
}

string Scene::getRootId() {
	return rootId;
}

Light* Scene::getLight(int index) {
	return lights[index];
}

Camera* Scene::getCamera(string key) {
	return cameras[key];
}

CGFtexture * Scene::getTexture(string key) {
	TexElem::iterator it = textures.find(key);
	if (it == textures.end()) {
		return NULL;
	} else {
		return it->second;
	}
}

Appearance* Scene::getAppearance(string key) {
	return appearances[key];
}

Animation* Scene::getAnimation(string key) {
	return animations[key];
}

Node* Scene::getNode(string key) {
	return graph[key];
}

void Scene::applyLights() {
	if (enabled) {
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, doublesided);

		float amb[4] = { amb_r, amb_g, amb_b, amb_a };
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, local);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

		for (unsigned int i = 0; i < lights.size(); i++) {
			lights[i]->displayLight();
		}

	} else {
		glDisable(GL_LIGHTING);
	}

}

void Scene::initCamera() {
	cameras[init_camera]->setCamera();
}

void Scene::initScene() {

	glEnable(GL_DEPTH_TEST);

	glClearColor(bckg_r, bckg_g, bckg_b, bckg_a);

	glPolygonMode(GL_FRONT_AND_BACK, drawmode);

	glShadeModel(shading);

	if (cullface == GL_NONE) {
		glDisable(GL_CULL_FACE);
	} else {
		glEnable(GL_CULL_FACE);
		glCullFace(cullface);
	}
	glFrontFace(cullorder);

	initCamera();

	AnimationElem::iterator it = animations.begin();
	int i = 0;
	string id;
	for (; it != animations.end(); it++) {
		animation_index.push_back(it->first);
		glutTimerFunc(ANIMATION_TIME, updateValues, i);
		i++;
	}

}

Scene::~Scene() {
}

Scene *Scene::getInstance() {
	if (instance == NULL)
		instance = new Scene();
	return instance;
}

void Scene::drawScene() {
	Node *ptr = getNode(rootId);
	stack<string> apps_stack;
	apps_stack.push("default");
	stack<string> ani_stack;
	ani_stack.push("default");
	ptr->processNode(apps_stack, ani_stack);
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, Scene::getInstance()->getDrawmode());

	Scene::getInstance()->initCamera();
	glTranslatef(obj_pos[0], obj_pos[1], -obj_pos[2]);
	glMultMatrixf(view_rotate);

	glPushMatrix();
	glMultMatrixf(Scene::getInstance()->getNode(Scene::getInstance()->getRootId())->getTransform());
	Scene::getInstance()->applyLights();
	glPopMatrix();

	glPushMatrix();
	Scene::getInstance()->drawScene();
	glPopMatrix();

	glutSwapBuffers();

	glFlush();

}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	Scene::WIDTH = w;
	Scene::HEIGHT = h;
	Scene::getInstance()->initCamera();

	glutPostRedisplay();
}

void idle(void) {
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}

string Scene::getInitCamera() {
	return init_camera;
}

Light* Scene::getLight(string id) {
	vector<Light *>::iterator it;
	for (it = lights.begin(); it != lights.end(); it++) {
		if ((*it)->getId() == id)
			return *it;
	}
	return NULL;
}

string Scene::getAnimationIndex(int index) {
	return animation_index[index];
}

void updateValues(int index) {
	string id = Scene::getInstance()->getAnimationIndex(index);
	Animation *ani = Scene::getInstance()->getAnimation(id);
	ani->updateValues();
	glutTimerFunc(ANIMATION_TIME, updateValues, index);

}

string Scene::findNextNameAvail(string id) {
	int i = 0;
	stringstream ss;

	do {
		i++;
		ss.str("");
		ss.clear();
		ss << id << i;
	} while (graph.find(ss.str()) != graph.end());
	return ss.str();
}

string Scene::findLastNameAvail(string id) {
	int i = 1;
	stringstream ss;
	ss << id << i;
	string tmp = ss.str();
	if (graph.find(id) != graph.end()) {
		return id;
	}
	if (graph.find(ss.str()) == graph.end()) // if it hasn't been defined
											 // "" is returned
		return "";
	do {
		tmp = ss.str();
		i++;
		ss.str("");
		ss.clear();
		ss << id << i;
	} while (graph.find(ss.str()) != graph.end());
	return tmp;
}

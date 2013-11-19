#include "Node.h"
#include <GL/glut.h>
#include "Scene.h"
#include "MyPrimitive.h"
#include "Appearance.h"
#include <stack>
#include <stdio.h>
#include <iostream>

using namespace std;

Node::Node() {

	id = "";
	nodeAppearance = "default";
	nodeAnimation = "default";
}

Node::Node(string id) {

	this->id = id;
	nodeAppearance = "default";
	nodeAnimation = "default";

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, transforms);
	glPopMatrix();
}

Node::Node(string id, float transforms[16]) {

	this->id = id;
	copy(&transforms[0], &transforms[16], this->transforms);
	nodeAppearance = "default";
	nodeAnimation = "default";
}

void Node::addRef(string ref) {

	refs.push_back(ref);
}

void Node::setAppearance(string appearance) {

	nodeAppearance = appearance;
}

void Node::setAnimation(string animation) {
	nodeAnimation = animation;
}

void Node::setTransform(float transforms[16]) {

	copy(&transforms[0], &transforms[16], this->transforms);
}

void Node::resetTransform() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, transforms);
}

void Node::addTranslate(float x, float y, float z) {

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(this->transforms);
	glTranslatef(x, y, z);
	glGetFloatv(GL_MODELVIEW_MATRIX, transforms);
	glPopMatrix();

}

void Node::addScale(float x, float y, float z) {

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(this->transforms);
	glScalef(x, y, z);
	glGetFloatv(GL_MODELVIEW_MATRIX, transforms);
	glPopMatrix();
}
void Node::addRotation(float angle, char axis) {

	float x = 0, y = 0, z = 0;

	if (axis == 'x') {
		x = 1;
	} else if (axis == 'y') {
		y = 1;
	} else {
		z = 1;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(this->transforms);
	glRotatef(angle, x, y, z);
	glGetFloatv(GL_MODELVIEW_MATRIX, transforms);
	glPopMatrix();
}
const float* Node::getTransform() {

	return transforms;
}

Appearance *Node::getAppearance() {
	return Scene::getInstance()->getAppearance(nodeAppearance);
}

vector<string> Node::getRefs() {

	return refs;
}

string Node::getAnimation() {
	return nodeAnimation;
}

string Node::getId() {

	return id;
}

Node::~Node() {
}

void Node::addPrimitive(MyPrimitive *prim) {
	prims.push_back(prim);
}

/**
 * function that processes a node's children
 */
void Node::processNode(stack<string> apps_stack, stack<string> ani_stack) {
	glPushMatrix();

	glMultMatrixf(transforms);
	if (getAppearance()->getId() == "default") {
		apps_stack.push(apps_stack.top());
	} else {
		apps_stack.push(getAppearance()->getId());
	}

	if (getAnimation() == "default") {
		ani_stack.push(ani_stack.top());
	} else {
		ani_stack.push(getAnimation());
	}

	glPushMatrix();
	if (ani_stack.top() != "default") {
		Point pt = Scene::getInstance()->getAnimation(ani_stack.top())->getPoint();
		glTranslatef(pt.getX(), pt.getY(), pt.getZ());
		glRotatef(Scene::getInstance()->getAnimation(ani_stack.top())->getRotation(), 0, 1, 0);
	}
	if (prims.size() != 0)
		drawPrims(apps_stack.top());

	glPopMatrix();
	vector<string>::iterator it;
	for (it = refs.begin(); it != refs.end(); it++) {
		Node *ptr = Scene::getInstance()->getNode((*it));
		ptr->processNode(apps_stack, ani_stack);
	}
	apps_stack.pop();
	ani_stack.pop();
	glPopMatrix();
}

/**
 * funtion responsible for drawing a node's primitive
 */
void Node::drawPrims(string appearance) {
	vector<MyPrimitive *>::const_iterator it;

	for (it = prims.begin(); it != prims.end(); it++) {
		Appearance *app = Scene::getInstance()->getAppearance(appearance);
		app->apply();
		(*it)->setAppearance(appearance);
		(*it)->draw();
		(*it)->clearAppearance();
	}
}

int Node::getType() {
	return NONE;
}

void Node::closeDefinition(stack<string> apps_stack) {

}

/*
 * DisplayList.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: diogo
 */

#include "DisplayList.h"
#include <GL/gl.h>
#include "Scene.h"
#include "MyPrimitive.h"
#include <stdio.h>

DisplayList::DisplayList() :
		Node() {
	list_id = glGenLists(1);
}

DisplayList::~DisplayList() {
	glDeleteLists(list_id, 1);
}

DisplayList::DisplayList(string id) :
		Node(id) {
	list_id = glGenLists(1);
}

DisplayList::DisplayList(string id, float transforms[16]) :
		Node(id, transforms) {
	list_id = glGenLists(1);
}

void DisplayList::processNode(stack<string> apps_stack,
		stack<string> ani_stack) {
	glCallList(list_id);
}

void DisplayList::drawPrims(string appearance) {
	vector<MyPrimitive *>::const_iterator it;

	for (it = prims.begin(); it != prims.end(); it++) {
		Appearance *app = Scene::getInstance()->getAppearance(appearance);
		app->apply();
		(*it)->setAppearance(appearance);
		(*it)->draw();
		(*it)->clearAppearance();
	}
}

void DisplayList::closeDefinition(stack<string> apps_stack) {
	glNewList(list_id, GL_COMPILE);
	glPushMatrix();
	glMultMatrixf(transforms);

	if (getAppearance()->getId() == "default") {
		apps_stack.push(apps_stack.top());
	} else {
		apps_stack.push(getAppearance()->getId());
	}

	glPushMatrix();
	if (prims.size() != 0)
		drawPrims(apps_stack.top());

	glPopMatrix();
	vector<string>::iterator it;
	for (it = refs.begin(); it != refs.end(); it++) {
		Node *ptr = Scene::getInstance()->getNode((*it));
		stack<string> tmp;
		tmp.push("default");	//stack tem que ter o primeiro elemento
		ptr->processNode(apps_stack, tmp);
	}
	apps_stack.pop();
	glPopMatrix();
	glEndList();
}

int DisplayList::getType() {
	return DISPLAY_LIST;
}

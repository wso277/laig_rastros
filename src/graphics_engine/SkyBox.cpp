/*
 * SkyBox.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#include "SkyBox.h"
#include "GL/gl.h"
#include <iostream>


SkyBox::SkyBox(string id, string prefix) {
	this->id = id;
	loadSkybox(prefix+id);
}
void SkyBox::loadSkybox(string prefix) {

	string str = prefix + "_front.jpg";
	SkyboxTexture[0] = new CGFtexture(str);
	str = prefix + "_left.jpg";
	SkyboxTexture[3] = new CGFtexture(str);
	str = prefix + "_back.jpg";
	SkyboxTexture[2] = new CGFtexture(str);
	str = prefix + "_right.jpg";
	SkyboxTexture[1] = new CGFtexture(str);
	str = prefix + "_top.jpg";
	SkyboxTexture[4] = new CGFtexture(str);
	str = prefix + "_down.jpg";
	SkyboxTexture[5] = new CGFtexture("../data/down.jpg");
}

void SkyBox::draw() {
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glColor4f(1, 1, 1, 1);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SkyboxTexture[0]->apply();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(20, -20, -20);
	glTexCoord2f(0, 0);
	glVertex3f(-20, -20, -20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 20, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 20, -20);
	glEnd();

	// Render the left quad
	SkyboxTexture[1]->apply();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(20, -20, 20);
	glTexCoord2f(0, 0);
	glVertex3f(20, -20, -20);
	glTexCoord2f(0, 1);
	glVertex3f(20, 20, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 20, 20);
	glEnd();

	// Render the back quad
	SkyboxTexture[2]->apply();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(-20, -20, 20);
	glTexCoord2f(0, 0);
	glVertex3f(20, -20, 20);
	glTexCoord2f(0, 1);
	glVertex3f(20, 20, 20);
	glTexCoord2f(1, 1);
	glVertex3f(-20, 20, 20);

	glEnd();

	// Render the right quad
	SkyboxTexture[3]->apply();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(-20, -20, -20);
	glTexCoord2f(0, 0);
	glVertex3f(-20, -20, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 20, 20);
	glTexCoord2f(1, 1);
	glVertex3f(-20, 20, -20);
	glEnd();

	// Render the top quad
	SkyboxTexture[4]->apply();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(-20, 20, -20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 20, 20);
	glTexCoord2f(0, 0);
	glVertex3f(20, 20, 20);
	glTexCoord2f(1, 0);
	glVertex3f(20, 20, -20);
	glEnd();

	// Render the bottom quad
	SkyboxTexture[5]->apply();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(-20, -20, -20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, -20, 20);
	glTexCoord2f(0, 0);
	glVertex3f(20, -20, 20);
	glTexCoord2f(1, 0);
	glVertex3f(20, -20, -20);
	glEnd();

	glPopAttrib();
}

string SkyBox::getId() {
	return id;
}

SkyBox::~SkyBox() {
}


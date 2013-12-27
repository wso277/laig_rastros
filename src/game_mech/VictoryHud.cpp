/*
 * VictoryHud.cpp
 *
 *  Created on: Dec 27, 2013
 *      Author: knoweat
 */

#include "VictoryHud.h"
#include <GL/gl.h>
#include "text3d.h"
#include <sstream>

VictoryHud::VictoryHud(int winner) :
		winner(winner) {
	t3dInit();
}

VictoryHud::~VictoryHud() {
}

void VictoryHud::draw() {
	stringstream ss;
	ss << "Victory of player " << winner << "!";
	glPushMatrix();
	glTranslatef(500, 350, 0);
	glScalef(50, 50, 50);
	glRotatef(180, 1, 0, 0);
	t3dDraw2D(ss.str(), 0, 0, 0.2f);
	glPopMatrix();
}

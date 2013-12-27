#include "Board.h"
#include <GL/gl.h>
#include "Scene.h"
#include <iostream>
#include "Interface.h"
#include "text3d.h"
#include "cmath"
#include "GameLogic.h"

#define LEV1_TOP_APP "lev1_board_top"
#define LEV1_SIDES_APP "lev1_board_sides"
#define LEV1_BOTTOM_APP "lev1_board_bottom"
#define LEV2_TOP_APP "lev2_board_top"
#define LEV2_SIDES_APP "lev2_board_sides"
#define LEV2_BOTTOM_APP "lev2_board_bottom"

#define ROT_FACTOR 0.01667

extern bool inSelectMode;
extern bool piece_moving;

float Board::rotation = 0.0;
float Board::midBoardRot = 0.0;
float time_last = 0.0;
float time_last2 = 0.0;

Board::Board() :
		MyPrimitive() {
	is_selectable = true;
	Scene::getInstance()->addTexture("board_body", "../data/wood.png");
	Scene::getInstance()->addTexture("board_top", "../data/board.png");

	// level 1

	Appearance *a;
	a = new Appearance(LEV1_TOP_APP);
	a->setTextProp("board_top", 1.0 / 3.5, 1.0 / 2.0);
	Scene::getInstance()->addAppearance(LEV1_TOP_APP, a);

	a = new Appearance(LEV1_SIDES_APP);
	a->setTextProp("board_body", 1.0 / 0.5, 1.0 / 3.75);
	Scene::getInstance()->addAppearance(LEV1_SIDES_APP, a);

	a = new Appearance(LEV1_BOTTOM_APP);
	a->setTextProp("board_body", 1.0 / 3.5, 1.0 / 2.0);
	Scene::getInstance()->addAppearance(LEV1_BOTTOM_APP, a);

	topLevel.setDims(7, 0.5, 4);
	topLevel.setAppearances(LEV1_TOP_APP, LEV1_BOTTOM_APP, LEV1_SIDES_APP);

	bottomLevel.setDims(7, 0.5, 4);
	bottomLevel.setAppearances(LEV1_TOP_APP, LEV1_BOTTOM_APP, LEV1_SIDES_APP);

	a = new Appearance(LEV2_TOP_APP);
	a->setTextProp("board_top", 1.0 / 1.5, 1.0 / 1.5);
	Scene::getInstance()->addAppearance(LEV2_TOP_APP, a);

	a = new Appearance(LEV2_SIDES_APP);
	a->setTextProp("board_body", 1.0 / 0.5, 1.0 / 1.5);
	Scene::getInstance()->addAppearance(LEV2_SIDES_APP, a);

	a = new Appearance(LEV2_BOTTOM_APP);
	a->setTextProp("board_body", 1.0 / 1.5, 1.0 / 1.5);
	Scene::getInstance()->addAppearance(LEV2_BOTTOM_APP, a);

	middleLevel.setDims(3, 0.5, 3);
	middleLevel.setAppearances(LEV2_TOP_APP, LEV2_BOTTOM_APP, LEV2_SIDES_APP);

	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 7; j++) {
			topMatrix[i][j].setSelectable(true);
			topMatrix[i][j].setVisibility(true);
			int top_name = 100 + (10 * (i + 1)) + (j + 1);
			topMatrix[i][j].setName(top_name);
			bottomMatrix[i][j].setSelectable(true);
			bottomMatrix[i][j].setVisibility(true);
			int bottom_name = 300 + (10 * (i + 4)) + (j + 1);
			bottomMatrix[i][j].setName(bottom_name);
		}
	}

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			middleMatrix[i][j].setSelectable(true);
			middleMatrix[i][j].setVisibility(true);
			int middle_name = 200 + (10 * (i + 3)) + (j + 3);
			middleMatrix[i][j].setName(middle_name);
		}
	}
	t3dInit();
	glutTimerFunc(ROT_FACTOR, updateRotation, 0);

	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	time_last = t.tv_nsec * 0.000000001;
}

Board::~Board() {
}

void Board::draw() {
	// draw top level
	glPushMatrix();
	glTranslatef(0, 2, -1.5);
	if (inSelectMode) {
		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 7; j++) {

				if (inSelectMode) {
					glPushName(topMatrix[i][j].getName());
				}

				glPushMatrix();
				glTranslatef(-3.0 + j, 0.0, -1.5 + i);
				topMatrix[i][j].draw();
				glPopMatrix();

				if (inSelectMode) {
					glPopName();
				}
			}
		}
	} else {
		glPushMatrix();
		glTranslatef(3.0, 0.8, -1.5);
		Scene::getInstance()->getAppearance("goal_homes")->apply();
		glRotatef(-rotation, 0, 1, 0);
		t3dDraw3D("2", 0, 0, 0.2f);
		glPopMatrix();
		topLevel.draw();
	}
	glPopMatrix();

	// draw bottom level
	glPushMatrix();
	glTranslatef(0, -2, 1.5);
	if (inSelectMode) {
		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 7; j++) {
				glPushName(bottomMatrix[i][j].getName());
				glPushMatrix();
				glTranslatef(-3.0 + j, 0.0, -1.5 + i);
				bottomMatrix[i][j].draw();
				glPopMatrix();
				glPopName();
			}
		}
	} else {
		glPushMatrix();
		glTranslatef(-3.0, 0.8, 1.5);
		Scene::getInstance()->getAppearance("goal_homes")->apply();
		glRotatef(rotation, 0, 1, 0);
		t3dDraw3D("1", 0, 0, 0.2f);
		glPopMatrix();
		bottomLevel.draw();
	}
	glPopMatrix();

	// draw middle level
	glPushMatrix();
	if (inSelectMode) {
		for (unsigned int i = 0; i < 3; i++) {
			for (unsigned int j = 0; j < 3; j++) {
				glPushName(middleMatrix[i][j].getName());
				glPushMatrix();
				glTranslatef(-1.0 + j, 0, -1.0 + i);
				middleMatrix[i][j].draw();
				glPopMatrix();
				glPopName();
			}
		}
	} else {
		glRotatef(midBoardRot, 0, 1, 0);
		middleLevel.draw();
	}
	glPopMatrix();
}

float Board::incrementRotation(float inc) {
	rotation += inc;
	if (rotation >= 360.0) {
		rotation -= 360.0;
	}
	return rotation;
}

void updateRotation(int index) {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	float timer = t.tv_nsec * 0.000000001;
	float sub = 0;

	if (timer < time_last) {
		sub = (1 - time_last) + timer;
	} else {
		sub = timer - time_last;
	}

	float ratio = sub / ROT_FACTOR;
	time_last = timer;

	Board::incrementRotation(ratio * 2);
	glutTimerFunc(ROT_FACTOR, updateRotation, 0);
}

void Board::rotateMiddleBoard() {
	time_last2 = 0.0;
	glutTimerFunc(ROT_FACTOR, updateMiddAnim, 0);
}

void updateMiddAnim(int index) {

	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	float timer = t.tv_nsec * 0.000000001 + floor(time_last2);

	if (Board::midBoardRot <= 90.0) {

		float sub = 0;

		if (timer < time_last2) {
			sub = ceil(time_last2) + timer;
		} else {
			sub = timer - time_last2;
		}

		float ratio = sub / ROT_FACTOR;
		time_last2 = timer;

		if (ratio > 90 - Board::midBoardRot) {
			ratio = 90 - Board::midBoardRot;
		}
		Board::midBoardRot += ratio;
		cout << Board::midBoardRot << endl;
		GameLogic::getInstance()->rotatePiecesInMiddle(ratio);
		glutTimerFunc(ROT_FACTOR, updateMiddAnim, 0);
	} else {
		piece_moving = false;
		GameLogic::getInstance()->endMiddleRot();
	}
}


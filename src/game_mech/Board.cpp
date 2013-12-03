#include "Board.h"
#include <GL/gl.h>
#include "Scene.h"

#define LEV1_TOP_APP "lev1_board_top"
#define LEV1_SIDES_APP "lev1_board_sides"
#define LEV1_BOTTOM_APP "lev1_board_bottom"
#define LEV2_TOP_APP "lev2_board_top"
#define LEV2_SIDES_APP "lev2_board_sides"
#define LEV2_BOTTOM_APP "lev2_board_bottom"

Board::Board() {
	Scene::getInstance()->addTexture("board_body", "..data/wood.png");
	Scene::getInstance()->addTexture("board_top", "..data/board.png");

	// level 1

	Appearance *a;
	a = new Appearance(LEV1_TOP_APP);
	a->setTextProp("board_top", 7, 4);
	Scene::getInstance()->addAppearance(LEV1_TOP_APP, a);

	a = new Appearance(LEV1_SIDES_APP);
	a->setTextProp("board_body", 0.1, 7);
	Scene::getInstance()->addAppearance(LEV1_SIDES_APP, a);

	a = new Appearance(LEV1_BOTTOM_APP);
	a->setTextProp("board_body", 7, 4);
	Scene::getInstance()->addAppearance(LEV1_BOTTOM_APP, a);

	topLevel.setDims(7, 0.1, 4);
	topLevel.setAppearances(LEV1_TOP_APP, LEV1_BOTTOM_APP, LEV1_SIDES_APP);

	bottomLevel.setDims(7, 0.1, 4);
	bottomLevel.setAppearances(LEV1_TOP_APP, LEV1_BOTTOM_APP, LEV1_SIDES_APP);

	a = new Appearance(LEV2_TOP_APP);
	a->setTextProp("board_top", 3, 3);
	Scene::getInstance()->addAppearance(LEV2_TOP_APP, a);

	a = new Appearance(LEV2_SIDES_APP);
	a->setTextProp("board_body", 0.1, 3);
	Scene::getInstance()->addAppearance(LEV2_SIDES_APP, a);

	a = new Appearance(LEV2_BOTTOM_APP);
	a->setTextProp("board_body", 3, 3);
	Scene::getInstance()->addAppearance(LEV2_BOTTOM_APP, a);

	middleLevel.setDims(3, 0.1, 3);
	middleLevel.setAppearances(LEV2_TOP_APP, LEV2_BOTTOM_APP, LEV2_SIDES_APP);

	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 7; j++) {
			topMatrix[i][j].setSelectable(true);
			topMatrix[i][j].setVisibility(false);
			int top_name = 100 + 10 * i + j;
			topMatrix[i][j].setName(top_name);
			bottomMatrix[i][j].setSelectable(true);
			bottomMatrix[i][j].setVisibility(false);
			int bottom_name = 300 + 10 * (i + 3) + j;
			topMatrix[i][j].setName(bottom_name);
		}
	}

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			middleMatrix[i][j].setSelectable(true);
			middleMatrix[i][j].setVisibility(false);
			int middle_name = 200 + 10 * (i + 2) + (j + 2);
			topMatrix[i][j].setName(middle_name);
		}
	}
}

Board::~Board() {
}

void Board::draw() {
	// draw top level
	glPushMatrix();
	glTranslatef(0, 2, -1.5);
	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 7; j++) {
			glPushMatrix();
			glTranslatef(i, 0, j + 0.5);
			topMatrix[i][j].draw();
			glPopMatrix();
		}
	}
	topLevel.draw();
	glPopMatrix();

	// draw bottom level
	glPushMatrix();
	glTranslatef(0, -2, 1.5);
	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 7; j++) {
			glPushMatrix();
			glTranslatef(i + 3, 0, j + 0.5);
			bottomMatrix[i][j].draw();
			glPopMatrix();
		}
	}
	bottomLevel.draw();
	glPopMatrix();

	// draw middle level
	glPushMatrix();
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i, 0, j);
			middleMatrix[i][j].draw();
			glPopMatrix();
		}
	}
	middleLevel.draw();
	glPopMatrix();
}

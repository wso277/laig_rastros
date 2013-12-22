/*
 * CurrentPiece.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include "CurrentPiece.h"
#include "Animation.h"
#include "Scene.h"

CurrentPiece::CurrentPiece() :
		Piece() {
	side_diff = 1.0;
	level_diff = 2;

	piece = new MyCylinder(0.5, 0.5, 1, 10, 10);
}

CurrentPiece::CurrentPiece(int col, int line, int level, float side_diff, float level_diff) :
		Piece(col, line, level, true, true, "default") {

	this->side_diff = side_diff;
	this->level_diff = level_diff;

	Animation *a;

	a = new Animation("descend", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, -level_diff, 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("descend", a);

	a = new Animation("leftdown", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(-side_diff, 0, side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("leftdown", a);

	a = new Animation("down", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, 0, side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("down", a);

	a = new Animation("rightdown", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(side_diff, 0, side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("rightdown", a);

	a = new Animation("left", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(-side_diff, 0, 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("left", a);

	a = new Animation("climb", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, level_diff, 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("climb", a);

	a = new Animation("right", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(side_diff, 0, 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("right", a);

	a = new Animation("leftup", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(-side_diff, 0, -side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("leftup", a);

	a = new Animation("up", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, 0, -side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("up", a);

	a = new Animation("rightup", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(side_diff, 0, -side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("rightup", a);

	AnimationElem::iterator it = Scene::getInstance()->getAnimations().begin();
	int i = 0;
	string id;
	for (; it != Scene::getInstance()->getAnimations().end(); it++) {
		Scene::getInstance()->getAnimationsIndex().push_back(it->first);
		i++;
	}

}

void CurrentPiece::draw() {
	Scene::getInstance()->getAppearance("goal_homes")->apply();
	drawPiece();
}

int CurrentPiece::getSideDiff() {
	return side_diff;
}

int CurrentPiece::getLevelDiff() {
	return level_diff;
}

void CurrentPiece::updateCoords() {
	switch (dir) {
	//leftdown
	case 1:
		col -= side_diff;
		line += side_diff;
		break;
		//down
	case 2:
		line += side_diff;
		break;
		//rightdown
	case 3:
		col += side_diff;
		line += side_diff;
		break;
		//left
	case 4:
		col -= side_diff;
		break;
		//climb
	case 5:
		level += level_diff;
		break;
		//right
	case 6:
		col += side_diff;
		break;
		//leftup
	case 7:
		col -= side_diff;
		line -= side_diff;
		break;
		//up
	case 8:
		col -= side_diff;
		break;
		//rightup
	case 9:
		col -= side_diff;
		line += side_diff;
		break;
		//descend
	case 0:
		level -= level_diff;
		break;
	}
}

CurrentPiece::~CurrentPiece() {
}


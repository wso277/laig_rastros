/*
 * CurrentPiece.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include "CurrentPiece.h"
#include "Animation.h"
#include "Scene.h"
#include <iostream>

CurrentPiece::CurrentPiece() :
		Piece() {
	side_diff = 1;
	level_diff = 2;

	piece = new MyCylinder(0.5, 0.5, 1, 10, 10);
}

CurrentPiece::CurrentPiece(int col, int line, int level, int side_diff, int level_diff) :
		Piece(col, line, level, true, true, "default") {

	this->side_diff = side_diff;
	this->level_diff = level_diff;

	Animation *a;

	a = new Animation("0descend", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, -(level_diff+1), 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("0descend", a);

	a = new Animation("1leftdown", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(-side_diff, 0, side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("1leftdown", a);

	a = new Animation("2down", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, 0, side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("2down", a);

	a = new Animation("3rightdown", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(side_diff, 0, side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("3rightdown", a);

	a = new Animation("4left", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(-side_diff, 0, 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("4left", a);

	a = new Animation("5climb", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, level_diff+1, 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("5climb", a);

	a = new Animation("6right", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(side_diff, 0, 0);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("6right", a);

	a = new Animation("7leftup", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(-side_diff, 0, -side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("7leftup", a);

	a = new Animation("8up", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(0, 0, -side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("8up", a);

	a = new Animation("9rightup", ANIMATION_SPAN, "linear");
	a->addPoint(0, 0, 0);
	a->addPoint(side_diff, 0, -side_diff);
	a->calculateDelta();

	Scene::getInstance()->addAnimation("9rightup", a);

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
		level -= level_diff;
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
		line -= side_diff;
		break;
		//rightup
	case 9:
		col += side_diff;
		line -= side_diff;
		break;
		//descend
	case 0:
		level += level_diff;
		break;
	}
}


void CurrentPiece::setDir(int dir) {
	this->dir = dir;
}
int CurrentPiece::getDir() {
	return dir;
}

CurrentPiece::~CurrentPiece() {
}


/*
 * Piece.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include "Piece.h"
#include "Scene.h"
#include "Interface.h"

extern bool inSelectMode;

Piece::Piece() :
		MyPrimitive() {
	col = 5;
	line = 3;
	level = 2;

	piece = new MyCylinder(0.5, 0.5, 1, 10, 10);
}

Piece::Piece(int col, int line, int level, bool select, bool visible, string appearance) :
		MyPrimitive() {

	this->col = col;
	this->line = line;
	this->level = level;

	piece = new MyCylinder(0.5, 0.5, 1, 10, 10);

	setAppearance(appearance);
	setSelectable(select);
	setVisibility(visible);
}

void Piece::setCol(int col) {
	this->col = col;
}

void Piece::setLine(int line) {
	this->line = line;
}

void Piece::setLevel(int level) {
	this->level = level;
}

int Piece::getCol() {
	return col;
}

int Piece::getLine() {
	return line;
}

int Piece::getLevel() {
	return level;
}

void Piece::draw() {
	Scene::getInstance()->getAppearance("piece_texture")->apply();
	drawPiece();
}

void Piece::drawPiece() {
	if (inSelectMode && is_selectable) {
		glPushName(000);
		glPushMatrix();
		glTranslatef(col - 4, -(level * 2) + 4, line - 4);
		glScalef(0.9, 0.5, 0.9);
		glRotatef(-90, 1, 0, 0);
		piece->draw();
		glPopMatrix();
	}

	if (!inSelectMode) {
		glPushMatrix();
		glTranslatef(col - 4, -(level * 2) + 4, line - 4);
		glScalef(0.9, 0.5, 0.9);
		glRotatef(-90, 1, 0, 0);
		piece->draw();
		glPopMatrix();
	}

}

Piece::~Piece() {
}


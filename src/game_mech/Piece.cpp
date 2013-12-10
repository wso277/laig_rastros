/*
 * Piece.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include "Piece.h"

Piece::Piece() : MyPrimitive() {
	col = 5;
	line = 3;
	level = 0;

	piece = new MyCylinder(0.5, 0.5, 1, 10, 10);
}

Piece::Piece(int col, int line, int level, bool select, bool visible, string appearance) : MyPrimitive() {

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
	glPushMatrix();
	piece->draw();
	glPopMatrix();

}

Piece::~Piece() {
}


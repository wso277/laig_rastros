/*
 * Piece.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include "Piece.h"

Piece::Piece() : Node("piece"){
	col = 5;
	line = 3;
	level = 0;

}

Piece::Piece(int col, int line, int level, string id, bool select, bool visible, string appearance) : Node(id) {

	this->col = col;
	this->line = line;
	this->level = level;

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

Piece::~Piece() {
}


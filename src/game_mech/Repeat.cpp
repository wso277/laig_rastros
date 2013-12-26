/*
 * Repeat.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#include "Repeat.h"
#include "GL/glui.h"

extern GLUI_Panel *Buttons;

Repeat* Repeat::repeat = NULL;

Repeat::Repeat() {
}

Repeat* Repeat::getInstance() {
	if (repeat == NULL) {
		repeat = new Repeat();
	}

	return repeat;
}

void Repeat::setTrail(list<Piece*> trail) {
	this->trail = trail;
}

void Repeat::start() {
	int col = trail.front()->getCol();
	int line = trail.front()->getLine();
	int level = trail.front()->getLevel();

	int ldiff = GameLogic::getInstance()->getPiece()->getLine() - line;
	int cdiff = GameLogic::getInstance()->getPiece()->getCol() - col;

	if (GameLogic::getInstance()->getPiece()->getLevel() > level && ldiff == 0 && cdiff == 0) {
		GameLogic::getInstance()->executeMove(5);
	} else if (GameLogic::getInstance()->getPiece()->getLevel() < level && ldiff == 0 && cdiff == 0) {
		GameLogic::getInstance()->executeMove(0);
	} else if (GameLogic::getInstance()->getPiece()->getLevel() == level) {
		if (ldiff == -1) {
			if (cdiff == -1) {
				GameLogic::getInstance()->executeMove(3);
			} else if (cdiff == 1) {
				GameLogic::getInstance()->executeMove(1);
			} else if (cdiff == 0) {
				GameLogic::getInstance()->executeMove(2);
			}
		} else if (ldiff == 1) {
			if (cdiff == -1) {
				GameLogic::getInstance()->executeMove(9);
			} else if (cdiff == 1) {
				GameLogic::getInstance()->executeMove(7);
			} else if (cdiff == 0) {
				GameLogic::getInstance()->executeMove(8);
			}
		} else if (ldiff == 0) {
			if (cdiff == -1) {
				GameLogic::getInstance()->executeMove(6);
			} else if (cdiff == 1) {
				GameLogic::getInstance()->executeMove(4);
			}
		}
	}

	trail.pop_front();
	if (!trail.empty()) {
		glutTimerFunc((ANIMATION_SPAN + 1) * 1000, repeat_handler, 0);
	} else {
		delete Repeat::getInstance();
		Buttons->enable();
	}
}

void repeat_handler(int id) {
	if (id == 0) {
		glutTimerFunc((ANIMATION_SPAN + 1) * 1000, repeat_handler, 1);
	} else {
		Repeat::getInstance()->start();
	}

}

void Repeat::popFirst() {
	trail.pop_front();
}

void Repeat::pushLastMove(Piece* piece) {
	trail.push_back(piece);
}

void Repeat::reset() {
	repeat = NULL;
}
Repeat::~Repeat() {
	trail.clear();
	reset();
}


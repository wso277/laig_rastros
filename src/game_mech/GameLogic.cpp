/*
 * GameLogic.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include "GameLogic.h"
#include "Client.h"
#include "utils.h"
#include <iostream>
#include <unistd.h>
#include "Scene.h"
#include "Piece.h"
#include "Node.h"

using namespace std;

GameLogic* GameLogic::game = NULL;

GameLogic* GameLogic::getInstance() {
	if (game == NULL) {
		game = new GameLogic();
	}

	return game;
}

GameLogic::GameLogic() {
	piece = new CurrentPiece(5, 3, 2, 1, 1);
	board = new Board();
	piece_selected = false;
	// used piece example
	//Piece *test_piece = new Piece(5, 3, 1, false, true, "default");

	Node *n = new Node("piece");
	n->setSelectable(true);
	n->addPrimitive(piece);
	Scene::getInstance()->getNode("scene")->addRef("piece");
	Scene::getInstance()->addNode("piece", n);
	Scene::getInstance()->getNode("scene")->addPrimitive(board);

	n = new Node("trail");
	n->setSelectable(false);
	Scene::getInstance()->getNode("scene")->addRef("trail");
	Scene::getInstance()->addNode("trail", n);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 7; j++) {
			topBoard[i][j] = '_';
			botBoard[i][j] = '_';
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 0 && j == 2) {
				midBoard[i][j] = 'O';
			} else {
				midBoard[i][j] = '_';
			}
		}
	}

}

Board* GameLogic::getBoard() {
	return board;
}

CurrentPiece* GameLogic::getPiece() {
	return piece;
}

void GameLogic::initGame() {
	//system("../rastros");

}

bool GameLogic::getPieceSelected() {
	return piece_selected;
}

void GameLogic::setPieceSelected(bool selected) {
	this->piece_selected = selected;
}

void GameLogic::executeMove(int dir) {

	Piece *p = new Piece(piece->getCol(), piece->getLine(), piece->getLevel(), false, true, "default");
	Scene::getInstance()->getNode("trail")->addPrimitive(p);
	trailPieces.push_back(p);

	if (piece->getLevel() == 1) {
		topBoard[piece->getLine()][piece->getCol()] = '#';
	} else if (piece->getLevel() == 2) {
		midBoard[piece->getLine() - 3][piece->getCol() - 3] = '#';
	} else {
		botBoard[piece->getLine() - 3][piece->getCol()] = '#';
	}

	switch (dir) {
	case 0:
		Scene::getInstance()->getNode("piece")->setAnimation("0descend");
		piece->setDir(0);
		Scene::getInstance()->getAnimation("0descend")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 0);
		break;
	case 1:
		Scene::getInstance()->getNode("piece")->setAnimation("1leftdown");
		piece->setDir(1);
		Scene::getInstance()->getAnimation("1leftdown")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 1);
		break;
	case 2:
		Scene::getInstance()->getNode("piece")->setAnimation("2down");
		piece->setDir(2);
		Scene::getInstance()->getAnimation("2down")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 2);
		break;
	case 3:
		Scene::getInstance()->getNode("piece")->setAnimation("3rightdown");
		piece->setDir(3);
		Scene::getInstance()->getAnimation("3rightdown")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 3);
		break;
	case 4:
		Scene::getInstance()->getNode("piece")->setAnimation("4left");
		piece->setDir(4);
		Scene::getInstance()->getAnimation("4left")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 4);
		break;
	case 5:
		Scene::getInstance()->getNode("piece")->setAnimation("5climb");
		piece->setDir(5);
		Scene::getInstance()->getAnimation("5climb")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 5);
		break;
	case 6:
		Scene::getInstance()->getNode("piece")->setAnimation("6right");
		piece->setDir(6);
		Scene::getInstance()->getAnimation("6right")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 6);
		break;
	case 7:
		Scene::getInstance()->getNode("piece")->setAnimation("7leftup");
		piece->setDir(7);
		Scene::getInstance()->getAnimation("7leftup")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 7);
		break;
	case 8:
		Scene::getInstance()->getNode("piece")->setAnimation("8up");
		piece->setDir(8);
		Scene::getInstance()->getAnimation("8up")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 8);
		break;
	case 9:
		Scene::getInstance()->getNode("piece")->setAnimation("9rightup");
		piece->setDir(9);
		Scene::getInstance()->getAnimation("9rightup")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 9);
		break;
	}

}

string GameLogic::getEncodedCharBoard() {

	if (piece->getLevel() == 1) {
		topBoard[piece->getLine()][piece->getCol()] = 'O';
	} else if (piece->getLevel() == 2) {
		midBoard[piece->getLine() - 3][piece->getCol() - 3] = 'O';
	} else {
		botBoard[piece->getLine() - 3][piece->getCol()] = 'O';
	}

	string enc_board = "[[";
	int i = 0;
	for (i; i < 2; i++) {
		for (int j = 0; j < 7; j++) {
			enc_board = enc_board + "cell(" + topBoard[i][j] + ",X,X)";
			if (j != 6) {
				enc_board = enc_board + ",";
			}
		}
		enc_board = enc_board + "],[";
	}

	for (int j = 0; j < 7; j++) {
		if (j > 1 && j < 5) {
			enc_board = enc_board + "cell(" + topBoard[i][j] + "," + midBoard[i - 2][j - 2] + ",X)";
		} else {
			enc_board = enc_board + "cell(" + topBoard[i][j] + ",X,X)";
		}
		if (j != 6) {
			enc_board = enc_board + ",";
		}
	}
	enc_board = enc_board + "],[";

	i++;
	for (int j = 0; j < 7; j++) {
		if (j > 1 && j < 5) {
			enc_board = enc_board + "cell(" + topBoard[i][j] + "," + midBoard[i - 2][j - 2] + "," + botBoard[i - 3][j]
					+ ")";
		} else {
			enc_board = enc_board + "cell(" + topBoard[i][j] + ",X," + botBoard[i - 3][j] + ")";
		}
		if (j != 6) {
			enc_board = enc_board + ",";
		}
	}
	enc_board = enc_board + "],[";

	i++;
	for (int j = 0; j < 7; j++) {
		if (j > 1 && j < 5) {
			enc_board = enc_board + "cell(X," + midBoard[i - 2][j - 2] + "," + botBoard[i - 3][j] + ")";
		} else {
			enc_board = enc_board + "cell(X,X," + botBoard[i - 3][j] + ")";
		}
		if (j != 6) {
			enc_board = enc_board + ",";
		}
	}
	enc_board = enc_board + "],[";

	i = 2;
	for (i; i < 4; i++) {
		for (int j = 0; j < 7; j++) {
			enc_board = enc_board + "cell(X,X," + botBoard[i][j] + ")";
			if (j != 6) {
				enc_board = enc_board + ",";
			}
		}
		if (i != 3) {
			enc_board = enc_board + "],[";
		} else {
			enc_board = enc_board + "]]";
		}
	}

	return enc_board;
}

string GameLogic::getTestPredicate(int index) {
	string predicate = "testMove(";
	switch (index) {
	case 0:
		predicate = predicate + "0";
		break;
	case 1:
		predicate = predicate + "1";
		break;
	case 2:
		predicate = predicate + "2";
		break;
	case 3:
		predicate = predicate + "3";
		break;
	case 4:
		predicate = predicate + "4";
		break;
	case 5:
		predicate = predicate + "5";
		break;
	case 6:
		predicate = predicate + "6";
		break;
	case 7:
		predicate = predicate + "7";
		break;
	case 8:
		predicate = predicate + "8";
		break;
	case 9:
		predicate = predicate + "9";
		break;
	case 10:
		predicate = predicate + "10";
		break;
	}

	predicate = predicate + "," + getEncodedCharBoard() + ").";
	return predicate;
}

void GameLogic::undo() {
	switch (piece->getLevel()) {
	case 1:
		topBoard[piece->getLine()][piece->getCol()] = '_';
		break;
	case 2:
		midBoard[piece->getLine()][piece->getCol()] = '_';
		break;
	case 3:
		botBoard[piece->getLine()][piece->getCol()] = '_';
		break;
	}

	piece->setLevel(trailPieces.back()->getLevel());
	piece->setLine(trailPieces.back()->getLine());
	piece->setCol(trailPieces.back()->getCol());

	switch (piece->getLevel()) {
	case 1:
		topBoard[piece->getLine()][piece->getCol()] = 'O';
		break;
	case 2:
		midBoard[piece->getLine()][piece->getCol()] = 'O';
		break;
	case 3:
		botBoard[piece->getLine()][piece->getCol()] = 'O';
		break;
	}

	deletePieceFromScene(trailPieces.back());
	trailPieces.pop_back();
}

void GameLogic::repeat() {
	list<Piece*> trail(trailPieces);
	trailPieces.clear();
	deletePieceFromScene(NULL);
	resetGame();
	trail.pop_front();
	repeatAux(trail);
}

void GameLogic::repeatAux(list<Piece*> trail) {

	int col = trail.front()->getCol();
	int line = trail.front()->getLine();
	int level = trail.front()->getLevel();

	int ldiff = piece->getLine() - line;
	int cdiff = piece->getCol() - col;

	if (piece->getLevel() > level && ldiff == 0 && cdiff == 0) {
		executeMove(5);
	} else if (piece->getLevel() < level && ldiff == 0 && cdiff == 0) {
		executeMove(0);
	} else if (piece->getLevel() == level) {
		if (ldiff == -1) {
			if (cdiff == -1) {
				executeMove(3);
			} else if (cdiff == 1) {
				executeMove(1);
			} else if (cdiff == 0) {
				executeMove(2);
			}
		} else if (ldiff == 1) {
			if (cdiff == -1) {
				executeMove(9);
			} else if (cdiff == 1) {
				executeMove(7);
			} else if (cdiff == 0) {
				executeMove(8);
			}
		} else if (ldiff == 0) {
			if (cdiff == -1) {
				executeMove(6);
			} else if (cdiff == 1) {
				executeMove(4);
			}
		}
	}

	trail.pop_front();
	if (!trail.empty()) {
		repeatAux(trail);
	}
}

void GameLogic::deletePieceFromScene(Piece* piece) {

	if (piece == NULL) {
		Scene::getInstance()->getNode("trail")->getPrims().clear();
	} else {
		for (int i = 0; i < Scene::getInstance()->getNode("trail")->getPrims().size(); i++) {
			if (((Piece*) (Scene::getInstance()->getNode("trail")->getPrims()[i]))->getLevel() == piece->getLevel()
					&& ((Piece*) (Scene::getInstance()->getNode("trail")->getPrims()[i]))->getCol() == piece->getCol()
					&& ((Piece*) (Scene::getInstance()->getNode("trail")->getPrims()[i]))->getLine()
							== piece->getLine()) {

				Scene::getInstance()->getNode("trail")->getPrims().erase(
						Scene::getInstance()->getNode("trail")->getPrims().begin() + i);
				break;
			}
		}
	}

}

void GameLogic::resetGame() {
	piece->setCol(5);
	piece->setLine(3);
	piece->setLevel(2);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 7; j++) {
			topBoard[i][j] = '_';
			botBoard[i][j] = '_';
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 0 && j == 2) {
				midBoard[i][j] = 'O';
			} else {
				midBoard[i][j] = '_';
			}
		}
	}
}

GameLogic::~GameLogic() {
}


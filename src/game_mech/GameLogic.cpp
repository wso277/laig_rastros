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

GameLogic::GameLogic() {
	piece = new CurrentPiece(5, 3, 2, 1, 2);
	board = new Board();

	// used piece example
	//Piece *test_piece = new Piece(5, 3, 1, false, true, "default");

	Node *n = new Node("piece");
	n->addPrimitive(piece);
	Scene::getInstance()->getNode("scene")->addRef("piece");
	Scene::getInstance()->addNode("piece", n);
	Scene::getInstance()->getNode("scene")->addPrimitive(board);

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

	Scene::getInstance()->getNode("piece")->setAnimation("left");
	//glutTimerFunc(ANIMATION_TIME, updateValues, 4);
}

Board* GameLogic::getBoard() {
	return board;
}

CurrentPiece* GameLogic::getPiece() {
	return piece;
}

void GameLogic::initGame() {
	system("../rastros");
}

string GameLogic::getEncodedCharBoard() {
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

	predicate = predicate +"," + getEncodedCharBoard() + ").";
	return predicate;
}

GameLogic::~GameLogic() {
}


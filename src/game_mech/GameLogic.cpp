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
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include "Repeat.h"

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
	current_player = 1;
	player1 = 0;
	player2 = 0;
	last_point = 0;
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
	startupCommunication();
}

Board* GameLogic::getBoard() {
	return board;
}

CurrentPiece* GameLogic::getPiece() {
	return piece;
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
		if (current_player == 1) {
			player1 += 2;
			last_point = 2;
			current_player = 2;
		} else if (current_player == 2) {
			player2 -= 2;
			last_point = -2;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("0descend");
		piece->setDir(0);
		Scene::getInstance()->getAnimation("0descend")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 0);
		break;
	case 1:
		if (current_player == 1) {
			player1 += 2;
			last_point = 2;
			current_player = 2;
		} else if (current_player == 2) {
			player2 -= 1;
			last_point = -1;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("1leftdown");
		piece->setDir(1);
		Scene::getInstance()->getAnimation("1leftdown")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 1);
		break;
	case 2:
		if (current_player == 1) {
			player1 += 1;
			last_point = 1;
			current_player = 2;
		} else if (current_player == 2) {
			last_point = 0;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("2down");
		piece->setDir(2);
		Scene::getInstance()->getAnimation("2down")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 2);
		break;
	case 3:
		if (current_player == 1) {
			last_point = 0;
			current_player = 2;
		} else if (current_player == 2) {
			last_point = 0;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("3rightdown");
		piece->setDir(3);
		Scene::getInstance()->getAnimation("3rightdown")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 3);
		break;
	case 4:
		if (current_player == 1) {
			player1 += 1;
			last_point = 1;
			current_player = 2;
		} else if (current_player == 2) {
			player2 -= 1;
			last_point = -1;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("4left");
		piece->setDir(4);
		Scene::getInstance()->getAnimation("4left")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 4);
		break;
	case 5:
		if (current_player == 1) {
			player1 -= 2;
			last_point = -2;
			current_player = 2;
		} else if (current_player == 2) {
			player2 += 2;
			last_point = 2;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("5climb");
		piece->setDir(5);
		Scene::getInstance()->getAnimation("5climb")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 5);
		break;
	case 6:
		if (current_player == 1) {
			player1 -= 1;
			last_point = -1;
			current_player = 2;
		} else if (current_player == 2) {
			player2 += 1;
			last_point = 1;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("6right");
		piece->setDir(6);
		Scene::getInstance()->getAnimation("6right")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 6);
		break;
	case 7:
		if (current_player == 1) {
			last_point = 0;
			current_player = 2;
		} else if (current_player == 2) {
			last_point = 0;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("7leftup");
		piece->setDir(7);
		Scene::getInstance()->getAnimation("7leftup")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 7);
		break;
	case 8:
		if (current_player == 1) {
			player1 -= 1;
			last_point = -1;
			current_player = 2;
		} else if (current_player == 2) {
			player2 += 1;
			last_point = 1;
			current_player = 1;
		}
		Scene::getInstance()->getNode("piece")->setAnimation("8up");
		piece->setDir(8);
		Scene::getInstance()->getAnimation("8up")->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, 8);
		break;
	case 9:
		if (current_player == 1) {
			player1 -= 2;
			last_point = -2;
			current_player = 2;
		} else if (current_player == 2) {
			player2 += 2;
			last_point = 2;
			current_player = 1;
		}
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

	if (current_player == 1) {
		player1 -= last_point;
		current_player = 2;
	} else if (current_player == 2) {
		player2 -= last_point;
		current_player = 1;
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

	Scene::getInstance()->getNode("trail")->removePiece(trailPieces.back());
	trailPieces.pop_back();
}

void GameLogic::repeat() {
	Repeat::getInstance()->setTrail(trailPieces);
	trailPieces.clear();
	Scene::getInstance()->getNode("trail")->removePiece(NULL);
	Repeat::getInstance()->pushLastMove(
			new Piece(piece->getCol(), piece->getLine(), piece->getLevel(), false, true, "default"));
	resetGame();
	Repeat::getInstance()->popFirst();
	Repeat::getInstance()->start();
}

void GameLogic::resetGame() {
	current_player = 1;
	player1 = 0;
	player2 = 0;
	last_point = 0;

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

void GameLogic::startupCommunication() {
	/*pid_t pid = fork();
	 switch(pid) {
	 case -1:
	 cout << "Error starting Prolog's component." << endl;
	 exit(-1);
	 case 0:
	 sleep(1);
	 break;
	 default:
	 execl("../rastros", NULL)
	 ;
	 }*/
	//cout << Client::getInstance()->sendRequest("test.\n") << endl;
}

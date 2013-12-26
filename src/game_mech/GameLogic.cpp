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
#include <string>

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
	cout << getTestPredicate(dir) << endl;
	/*string response = Client::getInstance()->sendRequest(getTestPredicate(dir));

	 if (response.compare("Success.") == 0) {

	 string animations[] = { "0descend", "1leftdown", "2down", "3rightdown",
	 "4left", "5climb", "6right", "7leftup", "8up", "9rightup" };

	 Piece *p = new Piece(piece->getCol(), piece->getLine(),
	 piece->getLevel(), false, true, "default");
	 Scene::getInstance()->getNode("trail")->addPrimitive(p);
	 trailPieces.push_back(p);

	 if (piece->getLevel() == 1) {
	 topBoard[piece->getLine()][piece->getCol()] = '#';
	 } else if (piece->getLevel() == 2) {
	 midBoard[piece->getLine() - 3][piece->getCol() - 3] = '#';
	 } else {
	 botBoard[piece->getLine() - 3][piece->getCol()] = '#';
	 }

	 Scene::getInstance()->getNode("piece")->setAnimation(animations[dir]);
	 piece->setDir(dir);
	 Scene::getInstance()->getAnimation(animations[dir])->resetTime();
	 glutTimerFunc(ANIMATION_TIME, updateValues, 0);
	 }*/
}

string GameLogic::getEncodedCharBoard() {

	if (piece->getLevel() == 1) {
		topBoard[piece->getLine()][piece->getCol()] = 'O';
	} else if (piece->getLevel() == 2) {
		midBoard[piece->getLine() - 3][piece->getCol() - 3] = 'O';
	} else {
		botBoard[piece->getLine() - 3][piece->getCol()] = 'O';
	}

	string enc_board = "[";

	for (unsigned int i = 0; i < 7; i++) {
		enc_board += "[";
		for (unsigned int j = 0; j < 7; j++) {
			enc_board += "cell(";
			if (i < 2 || (i == 2 && j < 2) || (i == 2 && j > 5)) {
				enc_board = enc_board + "'" + topBoard[i][j] + "','X','X'";
			} else if (i == 2 && j >= 2 && j > 5) {
				enc_board = enc_board + "'" + topBoard[i][j] + "','"
						+ midBoard[i - 2][j - 2] + "','X'";
			} else if (i == 4 && j >= 2 && j > 5) {
				enc_board = enc_board + "'X','" + midBoard[i - 2][j - 2] + "','"
						+ botBoard[i - 5][j] + "'";
			} else if (i == 4 && j >= 2 && j < 5) {
				enc_board = enc_board + "'X','X','" + botBoard[i - 5][j] + "'";
			} else if (i == 3) {
				if (j < 2 || j >= 5) {
					enc_board = enc_board + "'" + topBoard[i][j] + "','X','"
							+ botBoard[i - 5][j] + "'";
				} else {
					enc_board = enc_board + "'" + topBoard[i][j] + "','"
							+ midBoard[i - 2][j - 2] + "','"
							+ botBoard[i - 5][j] + "'";
				}
			}
			enc_board += ")";
			if (j != 6) {
				enc_board += ",";
			}
		}
		if (i != 6) {
			enc_board += ",";
		}
		enc_board += "]";
	}

	enc_board += "]";
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

	predicate = predicate + "," + getEncodedCharBoard() + ").\n";
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
		for (int i = 0;
				i < Scene::getInstance()->getNode("trail")->getPrims().size();
				i++) {
			if (((Piece*) (Scene::getInstance()->getNode("trail")->getPrims()[i]))->getLevel()
					== piece->getLevel()
					&& ((Piece*) (Scene::getInstance()->getNode("trail")->getPrims()[i]))->getCol()
							== piece->getCol()
					&& ((Piece*) (Scene::getInstance()->getNode("trail")->getPrims()[i]))->getLine()
							== piece->getLine()) {

				Scene::getInstance()->getNode("trail")->getPrims().erase(
						Scene::getInstance()->getNode("trail")->getPrims().begin()
								+ i);
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

void GameLogic::startupCommunication() {
	pid_t pid = fork();
	switch (pid) {
	case -1:
		cout << "Error starting Prolog's component." << endl;
		exit(-1);
	case 0:
		sleep(1);
		break;
	default:
		system("../rastros");
	}
	//Client::getInstance();
}

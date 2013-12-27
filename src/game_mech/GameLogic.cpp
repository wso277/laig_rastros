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
#include "Repeat.h"
#include "PointsHud.h"
#include "TimeHud.h"
#include "VictoryHud.h"

#define SUCCESS_MESSG "Success.\n"
#define VICTORY1_MESSG "Victory1.\n"
#define VICTORY2_MESSG "Victory2.\n"
#define FAILURE_MESSG "Failure.\n"

using namespace std;

bool piece_moving = false;
extern int GAME_MODE;
extern int DIFFICULTY_LEVEL;

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

	PointsHud *points = new PointsHud();
	Scene::getInstance()->getNode("UI")->addPrimitive(points);

	TimeHud *time = new TimeHud();
	Scene::getInstance()->getNode("UI")->addPrimitive(time);

	gameMode = 1;
	player1Name = "Human 1";
	player2Name = "Human 2";
	difficultyLevel = 1;
	piece_selected = false;
	current_player = 1;
	player1 = 0;
	player2 = 0;
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

	string response = Client::getInstance()->sendRequest(getTestPredicate(dir));
	cout << response << endl;
	if (response.compare(SUCCESS_MESSG) == 0 || response.compare(VICTORY1_MESSG) == 0
			|| response.compare(VICTORY2_MESSG) == 0) {

		string animations[] = { "0descend", "1leftdown", "2down", "3rightdown", "4left", "5climb", "6right", "7leftup",
				"8up", "9rightup" };

		if (dir != 0 && dir != 5) {
			Piece *p = new Piece(piece->getCol(), piece->getLine(), piece->getLevel(), false, true, "default");
			Scene::getInstance()->getNode("trail")->addPrimitive(p);
			trailPieces.push_back(p);

			if (piece->getLevel() == 1) {
				topBoard[piece->getLine() - 1][piece->getCol() - 1] = '#';
			} else if (piece->getLevel() == 2) {
				midBoard[piece->getLine() - 3][piece->getCol() - 3] = '#';
			} else {
				botBoard[piece->getLine() - 4][piece->getCol() - 1] = '#';
			}
		}

		Scene::getInstance()->getNode("piece")->setAnimation(animations[dir]);
		piece->setDir(dir);
		Scene::getInstance()->getAnimation(animations[dir])->resetTime();
		glutTimerFunc(ANIMATION_TIME, updateValues, dir);
		piece_moving = true;
		assignPoints(dir);
		if (response.compare(VICTORY1_MESSG) == 0) {
			cout << "Victory player 1" << endl;
			VictoryHud *vict = new VictoryHud(1);
			Scene::getInstance()->getNode("UI")->addPrimitive(vict);
			piece_moving = true;
		} else if (response.compare(VICTORY2_MESSG) == 0) {
			cout << "Victory player2" << endl;
			VictoryHud *vict = new VictoryHud(2);
			Scene::getInstance()->getNode("UI")->addPrimitive(vict);
			piece_moving = true;
		}
	}
}

void GameLogic::assignPoints(int dir) {
	switch (dir) {
	case 0:
		if (current_player == 1) {
			player1 += 2;
			last_point.push_back(2);
		} else {
			player2 -= 2;
			last_point.push_back(-2);
		}
		break;
	case 1:
		if (current_player == 1) {
			player1 += 2;
			last_point.push_back(2);
		} else {
			player2 -= 2;
			last_point.push_back(-2);
		}
		break;
	case 2:
		if (current_player == 1) {
			player1 += 1;
			last_point.push_back(1);
		} else {
			player2 -= 1;
			last_point.push_back(-1);
		}
		break;
	case 3:
		last_point.push_back(0);
		break;
	case 4:
		if (current_player == 1) {
			player1 += 1;
			last_point.push_back(1);
		} else {
			player2 -= 1;
			last_point.push_back(-1);
		}
		break;
	case 5:
		if (current_player == 1) {
			player1 -= 2;
			last_point.push_back(-2);
		} else {
			player2 += 2;
			last_point.push_back(2);
		}
		break;
	case 6:
		if (current_player == 1) {
			player1 -= 1;
			last_point.push_back(-1);
		} else {
			player2 += 1;
			last_point.push_back(1);
		}
		break;
	case 7:
		last_point.push_back(0);
		break;
	case 8:
		if (current_player == 1) {
			player1 -= 1;
			last_point.push_back(-1);
		} else {
			player2 += 1;
			last_point.push_back(1);
		}
		break;
	case 9:
		if (current_player == 1) {
			player1 -= 2;
			last_point.push_back(-2);
		} else {
			player2 += 2;
			last_point.push_back(2);
		}
		break;
	}

	if (current_player == 1 && dir != 0 && dir != 5) {
		current_player = 2;
	} else if (current_player == 2 && dir != 0 && dir != 5) {
		current_player = 1;
	}
}

string GameLogic::getEncodedCharBoard() {

	if (piece->getLevel() == 1) {
		topBoard[piece->getLine() - 1][piece->getCol() - 1] = 'O';
	} else if (piece->getLevel() == 2) {
		midBoard[piece->getLine() - 3][piece->getCol() - 3] = 'O';
	} else {
		botBoard[piece->getLine() - 4][piece->getCol() - 1] = 'O';
	}

	string enc_board = "[";

	for (unsigned int i = 0; i < 7; i++) {
		enc_board += "[";
		for (unsigned int j = 0; j < 7; j++) {
			enc_board += "cell(";
			if (i == 0 && j == 6) {
				enc_board = enc_board + "'2','X','X'";
			} else if (i == 6 && j == 0) {
				enc_board = enc_board + "'X','X','1'";
			} else if (i < 2 || (i == 2 && j < 2) || (i == 2 && j >= 5)) {
				enc_board = enc_board + "'" + topBoard[i][j] + "','X','X'";
			} else if (i > 4 || (i == 4 && j < 2) || (i == 4 && j >= 5)) {
				enc_board = enc_board + "'X','X','" + botBoard[i - 3][j] + "'";
			} else if (i == 2 && j >= 2 && j < 5) {
				enc_board = enc_board + "'" + topBoard[i][j] + "','" + midBoard[i - 2][j - 2] + "','X'";
			} else if (i == 4 && j >= 2 && j < 5) {
				enc_board = enc_board + "'X','" + midBoard[i - 2][j - 2] + "','" + botBoard[i - 3][j] + "'";
			} else if (i == 3) {
				if (j < 2 || j >= 5) {
					enc_board = enc_board + "'" + topBoard[i][j] + "','X','" + botBoard[i - 3][j] + "'";
				} else {
					enc_board = enc_board + "'" + topBoard[i][j] + "','" + midBoard[i - 2][j - 2] + "','"
							+ botBoard[i - 3][j] + "'";
				}
			}
			enc_board += ")";
			if (j != 6) {
				enc_board += ",";
			}
		}
		enc_board += "]";
		if (i != 6) {
			enc_board += ",";
		}
	}

	if (piece->getLevel() == 1) {
		topBoard[piece->getLine() - 1][piece->getCol() - 1] = '_';
	} else if (piece->getLevel() == 2) {
		midBoard[piece->getLine() - 3][piece->getCol() - 3] = '_';
	} else {
		botBoard[piece->getLine() - 4][piece->getCol() - 1] = '_';
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
		topBoard[piece->getLine() - 1][piece->getCol() - 1] = '_';
		break;
	case 2:
		midBoard[piece->getLine() - 3][piece->getCol() - 3] = '_';
		break;
	case 3:
		botBoard[piece->getLine() - 4][piece->getCol() - 1] = '_';
		break;
	}

	if (current_player == 1) {
		player2 -= last_point.back();
		last_point.pop_back();
		current_player = 2;
	} else if (current_player == 2) {
		player1 -= last_point.back();
		last_point.pop_back();
		current_player = 1;
	}

	piece->setLevel(trailPieces.back()->getLevel());
	piece->setLine(trailPieces.back()->getLine());
	piece->setCol(trailPieces.back()->getCol());

	Scene::getInstance()->getNode("trail")->removePiece(trailPieces.back());
	trailPieces.pop_back();
}

void GameLogic::repeat() {
	Repeat::getInstance()->setTrail(trailPieces);
	trailPieces.clear();
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
	last_point.clear();
	gameMode = GAME_MODE + 1;
	string queryMode = "sGameMode(";
	string queryDifficulty = "sDifficultyLevel(";
	switch (gameMode) {
	case 1:
		player1Name = "Human 1";
		player2Name = "Human 2";
		break;
	case 2:
		player1Name = "Human";
		player2Name = "Computer";
		queryMode = queryMode + "2).\n";
		break;
	case 3:
		player1Name = "Computer";
		player2Name = "Human";
		queryMode = queryMode + "3).\n";
		break;
	case 4:
		player1Name = "Computer 1";
		player2Name = "Computer 2";
		queryMode = queryMode + "4).\n";
		break;
	}
	difficultyLevel = DIFFICULTY_LEVEL + 1;

	if (gameMode != 1) {
		switch (difficultyLevel) {
		case 1:
			queryDifficulty = queryDifficulty + "1).\n";
			break;
		case 2:
			queryDifficulty = queryDifficulty + "2).\n";
			break;
		case 3:
			queryDifficulty = queryDifficulty + "3).\n";
			break;
		case 4:
			queryDifficulty = queryDifficulty + "4).\n";
			break;
		}
		Client::getInstance()->sendRequest(queryMode);
		Client::getInstance()->sendRequest(queryDifficulty);

	}

	piece->setCol(5);
	piece->setLine(3);
	piece->setLevel(2);

	Scene::getInstance()->getNode("trail")->removePiece(NULL);

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

int GameLogic::getPlayer1() {
	return player1;
}

int GameLogic::getPlayer2() {
	return player2;
}

string GameLogic::getPlayer1Name() {
	return player1Name;
}

string GameLogic::getPlayer2Name() {
	return player2Name;
}

int GameLogic::getCurrentPlayer() {
	return current_player;
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

}

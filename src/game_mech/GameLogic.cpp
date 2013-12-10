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

using namespace std;

GameLogic::GameLogic() {
	piece = new CurrentPiece(5, 3, 0, 1, 1);
	board = new Board();

	initGame();
}

void GameLogic::initGame() {

	string host = askHost();
	Client::getInstance()->setHost(host);

	int port = askPort();
	Client::getInstance()->setPort(port);

	Client::getInstance()->startConnection();
}

GameLogic::~GameLogic() {
}


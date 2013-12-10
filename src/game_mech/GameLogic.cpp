/*
 * GameLogic.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#include "GameLogic.h"
#include "Client.h"

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

string askHost() {
	string host;
	cout << "Insert Host Name: " << endl;
	cin >> host;

	return host;
}

int askPort() {
	string port_str;
	cout << "Insert Port Number: " << endl;
	getline(cin, port_str);

	while (!isdigit(port_str[0])) {
		cout << "Not a number. Please insert a number: " << endl;
		getline(cin, port_str);
	}

	int port = atoi(port_str.c_str());

	return port;
}

GameLogic::~GameLogic() {
	// TODO Auto-generated destructor stub
}


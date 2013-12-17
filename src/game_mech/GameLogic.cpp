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

using namespace std;

GameLogic::GameLogic() {
	piece = new CurrentPiece(5, 3, 2, 1, 1);
	board = new Board();

	/*Piece *test_piece = new Piece(5, 3, 1, false, true, "default");

	Scene::getInstance()->getNode("scene")->addPrimitive(test_piece);

	Piece *test_piece1 = new Piece(5, 3, 3, false, true, "default");

	Scene::getInstance()->getNode("scene")->addPrimitive(test_piece1);*/

	Scene::getInstance()->getNode("scene")->addPrimitive(piece);
	Scene::getInstance()->getNode("scene")->addPrimitive(board);
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

GameLogic::~GameLogic() {
}


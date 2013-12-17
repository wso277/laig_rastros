/*
 * GameLogic.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include "CurrentPiece.h"
#include "Board.h"

class GameLogic {
	CurrentPiece *piece;
	Board *board;
public:
	GameLogic();
	void initGame();
	Board* getBoard();
	CurrentPiece* getPiece();
	virtual ~GameLogic();
};

#endif /* GAMELOGIC_H_ */

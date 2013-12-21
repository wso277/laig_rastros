/*
 * GameLogic.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include "CurrentPiece.h"
#include "Piece.h"
#include "Board.h"

class GameLogic {
	CurrentPiece *piece;
	vector<Piece> trailPieces;
	Board *board;
public:
	GameLogic();
	virtual ~GameLogic();
	void initGame();
	Board* getBoard();
	CurrentPiece* getPiece();
	void getEncodedCharBoard

};

#endif /* GAMELOGIC_H_ */

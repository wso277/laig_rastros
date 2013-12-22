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
	char topBoard[4][7];
	char midBoard[3][3];
	char botBoard[4][7];
public:
	GameLogic();
	virtual ~GameLogic();
	void initGame();
	Board* getBoard();
	CurrentPiece* getPiece();
	string getEncodedCharBoard();
	string getTestPredicate(int index);

};

#endif /* GAMELOGIC_H_ */

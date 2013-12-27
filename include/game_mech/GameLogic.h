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
#include <list>

void animTrailScale(int index);

class GameLogic {
	CurrentPiece *piece;
	list<Piece*> trailPieces;
	Board *board;
	char topBoard[4][7];
	char midBoard[3][3];
	char botBoard[4][7];
	bool piece_selected;
	int current_player;
	int player1;
	int player2;
	string player1Name;
	string player2Name;
	vector<int> last_point;
	int gameMode;
	int difficultyLevel;
	static GameLogic *game;

	void startupCommunication();
	vector<Piece*> getMiddlePieces();
	void aiMove(int current_player);

public:
	static GameLogic* getInstance();
	GameLogic();
	virtual ~GameLogic();
	void initGame();
	Board* getBoard();
	CurrentPiece* getPiece();
	string getEncodedCharBoard();
	string getTestPredicate(int index);
	void executeMove(int dir);
	bool getPieceSelected();
	void setPieceSelected(bool selected);
	void undo();
	void repeat();
	void resetGame();
	int getPlayer1();
	int getPlayer2();
	string getPlayer1Name();
	string getPlayer2Name();
	int getCurrentPlayer();
	void assignPoints(int dir);
	void rotateMidBoard();
	void rotatePiecesInMiddle(float factor);
	void endMiddleRot();
	bool existPossibleMoves();
	void finishedMoving();
	void executeAIMove(int dir);

	friend void animTrailScale(int index);
};

#endif /* GAMELOGIC_H_ */

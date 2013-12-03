/*
 * CurrentPiece.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef CURRENTPIECE_H_
#define CURRENTPIECE_H_

#include "Piece.h"

#define ANIMATION_SPAN 3

class CurrentPiece: public Piece {
private:
	float side_diff;
	float level_diff;
public:
	CurrentPiece();
	CurrentPiece(int col, int line, int level, float side_diff, float level_diff);
	int getSideDiff();
	int getLevelDiff();
	void updateCoords(int dir);
	virtual ~CurrentPiece();
};

#endif /* CURRENTPIECE_H_ */

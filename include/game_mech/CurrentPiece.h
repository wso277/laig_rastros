/*
 * CurrentPiece.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef CURRENTPIECE_H_
#define CURRENTPIECE_H_

#include "Piece.h"

#define ANIMATION_SPAN 1

class CurrentPiece: public Piece {
private:
	int side_diff;
	int level_diff;
	int dir;
public:
	CurrentPiece();
	CurrentPiece(int col, int line, int level, int side_diff, int level_diff);
	int getSideDiff();
	int getLevelDiff();
	void updateCoords();
	void draw();
	void setDir(int dir);
	int getDir();
	virtual ~CurrentPiece();
	void addTextures();
};

#endif /* CURRENTPIECE_H_ */

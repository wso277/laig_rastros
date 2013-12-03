/*
 * Piece.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef PIECE_H_
#define PIECE_H_

#include <string.h>
#include "MyPrimitive.h"
#include "MyCylinder.h"

using namespace std;

class Piece : public MyPrimitive {
protected:
	int col;
	int line;
	int level;
	MyCylinder *piece;
public:
	Piece();
	Piece(int col, int line, int level, bool select, bool visible, string appearance);
	void setCol(int col);
	void setLine(int line);
	void setLevel(int level);
	int getCol();
	int getLine();
	int getLevel();
	void draw();
	virtual ~Piece();
};

#endif /* PIECE_H_ */

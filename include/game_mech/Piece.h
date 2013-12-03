/*
 * Piece.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef PIECE_H_
#define PIECE_H_

#include <string.h>
#include "Node.h"

using namespace std;

class Piece : public Node {
private:
	int col;
	int line;
	int level;
public:
	Piece();
	Piece(int col, int line, int level, string id, bool select, bool visible, string appearance);
	void setCol(int col);
	void setLine(int line);
	void setLevel(int level);
	int getCol();
	int getLine();
	int getLevel();
	virtual ~Piece();
};

#endif /* PIECE_H_ */

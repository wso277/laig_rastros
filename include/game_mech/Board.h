/*
 * Board.h
 *
 *  Created on: Dec 3, 2013
 *      Author: knoweat
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "MyParallelepiped.h"
#include "MyPrimitive.h"
#include "Plane.h"

void updateRotation(int index);

class Board : public MyPrimitive {
	static float rotation;
	MyParallelepiped topLevel, middleLevel, bottomLevel;
	Plane topMatrix[4][7];
	Plane middleMatrix[3][3];
	Plane bottomMatrix[4][7];
public:
	Board();
	virtual ~Board();
	void draw();
	static float incrementRotation(float inc);
	bool rotateBoard(int boardIndex);
};

#endif /* BOARD_H_ */

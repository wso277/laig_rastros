/*
 * Repeat.h
 *
 *  Created on: Dec 26, 2013
 *      Author: wso277
 */

#ifndef REPEAT_H_
#define REPEAT_H_

#include "Piece.h"
#include "GameLogic.h"
#include <list>

using namespace std;

void repeat_handler(int id);

class Repeat {
private:
	list<Piece*> trail;
	static Repeat* repeat;
public:
	static Repeat* getInstance();
	static void reset();
	Repeat();
	void setTrail(list<Piece*> trail);
	void start();
	void popFirst();
	void pushLastMove(Piece* piece);
	virtual ~Repeat();
};

#endif /* REPEAT_H_ */

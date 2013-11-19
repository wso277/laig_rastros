/*
 * Point.h
 *
 *  Created on: Oct 22, 2013
 *      Author: wso277
 */

#ifndef POINT_H_
#define POINT_H_

class Point {
private:
	float x;
	float y;
	float z;
public:
	Point();
	Point(float x, float y, float z);
	float getX();
	float getY();
	float getZ();
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	virtual ~Point();
};

#endif /* POINT_H_ */

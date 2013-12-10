/*
 * SocketError.h
 *
 *  Created on: Dec 10, 2013
 *      Author: wso277
 */

#ifndef SOCKETERROR_H_
#define SOCKETERROR_H_

#include <string>

using namespace std;

class SocketError {
private:
	string message;
public:
	SocketError();
	SocketError(string message);
	string getMessage();
	virtual ~SocketError();
};

#endif /* SOCKETERROR_H_ */

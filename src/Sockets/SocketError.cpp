/*
 * SocketError.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: wso277
 */

#include "SocketError.h"

SocketError::SocketError() {
}

SocketError::SocketError(string message) {
	this->message = message;
}
string SocketError::getMessage() {
	return message;
}

SocketError::~SocketError() {
}


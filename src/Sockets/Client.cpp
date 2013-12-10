/*
 * Client.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: wso277
 */

#include "Client.h"
#include "SocketError.h"

Client * Client::client = NULL;

Client::Client() {
	portNo = 0;
	sockFd = 0;
	server = NULL;
}

void Client::setHost(string host) {
	server = gethostbyname(host.c_str());
	if (server == NULL) {
		throw SocketError("Server doesn't  exist!");
	}
}
void Client::setPort(int port) {
	this->portNo = portNo;
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd < 0) {
		throw SocketError("Error opening Socket!");
	}
}

Client* Client::getInstance() {
	if (client == NULL) {
		client = new Client();
	}
	return client;
}

void Client::startConnection() {

	if (portNo == 0 || sockFd == 0 || server == NULL) {
		throw SocketError("Socket variables not started");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portNo);

	if (connect(sockFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		throw SocketError("Error connecting!");
	}
}

char* Client::sendRequest(string request) {
	int n = 0;
	n = write(sockFd, request.c_str(), request.size());
	if (n != request.size()) {
		throw SocketError("Error writing to socket!");
	}

	char *reply;
	reply = (char *) malloc(MAX_REPLY_SIZE);
	memset(reply, '0', MAX_REPLY_SIZE);

	n = 0;
	do {
		read(sockFd, &reply[n++], 1);
		if (n > MAX_REPLY_SIZE) {
			throw SocketError("Message Incomplete!");
		}
	} while (reply[n - 1] != '.');

	return reply;
}

void Client::closeConnection() {
	close(sockFd);
}

Client::~Client() {
}


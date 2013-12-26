/*
 * Client.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: wso277
 */

#include "Client.h"
#include "SocketError.h"
#include <iostream>

using namespace std;

Client * Client::client = NULL;

Client::Client() {
	portNo = 30075;
	sockFd = 0;
	server = NULL;

	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd <= 0) {
		throw SocketError("Error opening Socket!");
	}

	string host = "localhost";
	server = gethostbyname(host.c_str());

	if (portNo == 0 || server == NULL) {
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

Client* Client::getInstance() {
	if (client == NULL) {
		client = new Client();
	}
	return client;
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
	} while (reply[n - 1] != '\n');
	reply[n] = '\0';

	return reply;
}

void Client::closeConnection() {
	close(sockFd);
}

Client::~Client() {
	cout << sendRequest("fail.\n") << endl;
	close(sockFd);
}


/*
 * Client.h
 *
 *  Created on: Dec 10, 2013
 *      Author: wso277
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>

#define MAX_REPLY_SIZE 256

using namespace std;

class Client {
private:
	int sockFd, portNo;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	static Client *client;
public:
	Client();
	Client(string host, int portNo);
	static Client* getInstance();
	char* sendRequest(string request);
	void setHost(string host);
	void setPort(int port);
	void startConnection();
	void closeConnection();
	virtual ~Client();
};

#endif /* CLIENT_H_ */

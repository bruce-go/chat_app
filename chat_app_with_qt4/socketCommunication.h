#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QMessageBox>

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "client_server_comm.h"

class SocketCommunication
{
public:
	// SocketCommunication();

	int socket_connect();

	int readFromServer(int sockfd, void *readBuf, size_t size);

	void writeToServer(int sockfd, void *writeBuf, size_t size);

};

#endif
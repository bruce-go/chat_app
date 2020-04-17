#include "socketCommunication.h"

int SocketCommunication::socket_connect()
{
	int sockfd;
	int rtnVal = 0;

	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//socket创建失败的错误还未处理，这会与connect函数返回的-1相互干扰，导致歧义

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);

	//服务器ip地址和端口号都可以打桩，127.0.0.1， 9877
	inet_pton(AF_INET, SERVER_IP_ADDRESS, &servaddr.sin_addr);
	
	rtnVal = connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

	QMessageBox msgBox;

	if(rtnVal == -1)//出现错误
	{
		if(errno == ETIMEDOUT)
		{
			//超时未收到服务器的响应
			msgBox.setText("Cann't connect to the server, please check your network.");
		}
		else if(errno == ECONNREFUSED)
		{
			//服务器主机上没有进程正在监听该连接，可以认为服务器程序没有启动
			msgBox.setText("Cann't connect to the server, the server is not running.");
		}
		else if(errno == EHOSTUNREACH || errno == ENETUNREACH)
		{
			//主机或者网络不可达
			msgBox.setText("Cann't connect to the server, the server host is unreachable.");
		}
		else
		{
			msgBox.setText("Cann't connect to the server, unknown error.");
		}

		msgBox.exec();

		return rtnVal;
	}
	
	return sockfd;
}

int SocketCommunication::readFromServer(int sockfd, void *readBuf, size_t size)
{
	ssize_t n;
	// char buf[MAXLINE];
	n = read(sockfd, readBuf, size);
// again:
	/*while( (n = read(sockfd, readBuf, MAXLINE)) > 0)
	{
		// write(sockfd, buf, n);
	}*/

	/*if(n < 0 && errno == EINTR)

	{
		goto again;
	}
	else if(n < 0)
	{
		fprintf(stderr, "str_echo: read error\n");
	}*/
	return n;
}

void SocketCommunication::writeToServer(int sockfd, void *writeBuf, size_t size)
{
	write(sockfd, writeBuf, size);
}

/*void SocketCommunication::ioMultiplexing(FILE *fp, int sockfd)
{
	fd_set rdSet;
	int maxfdpl;

	for( ; ; )
	{
		FD_SET(fileno(fp), &rdSet);
		FD_SET(sockfd, &rdSet);
		maxfdpl = max(fileno(fp), sockfd) + 1;
		select(maxfdpl, &rdSet, NULL, NULL, NULL);

		if(FD_ISSET(sockfd. &rdSet))
		{

		}

		if(FD_ISSET(fileno(fp), &rdSet))
		{
			
		}
	}
}*/

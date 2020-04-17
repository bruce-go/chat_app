#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //必须
#include <errno.h>

#include "rdwrt_lib.h"

//去读取一次，在现有套接字描述符缓冲区中读到多少就是多少
int readOnce(int fd, void *buf, unsigned int len)
{
	int rdCnt = 0;
	int rdLen;
	char *ptr;

	ptr = buf;
	rdLen = len;

	while( (rdCnt = read(fd, ptr, rdLen)) < 0)
	{
		if(errno == EINTR) //如果是读调用被信号中断了，则继续读
		{
			continue;
		}
		else
		{
			return -1; //否则，读出错，返回
		}
	}

	return rdCnt;//读到的实际字节数
}


/*读去指定长度的字节，直到遇到EOF或读取完成为止
*如果读到的字节数小于指定的字节数，则会阻塞
*/
int readn(int fd, void *buf, unsigned int len)
{
	int rdCnt;
	int rdLen;
	char *ptr;

	ptr = buf;
	rdLen = len;

	while(rdLen > 0)
	{
		printf("point 0.1, server start to read from client\n");
		rdCnt = read(fd, ptr, rdLen);//如果TCP缓冲区中的消息比rdLen小，
		//则第一次读取后会循环回来读第二次，此时由于缓冲区中的消息已被读走(什么时候会有EOF？)，导致read调用阻塞在这里，使得服务器没法与客户端交互
		printf("point 0.1, server finished read from client, rdCnt = %d\n", rdCnt);
		if(rdCnt < 0)//什么情况下会读出错？对端连接已经关闭时？
		{
			if(errno == EINTR) //如果是读调用被信号中断了，则继续读
			{
				continue;
			}
			else
			{
				return -1; //否则，读出错，返回
			}
		}

		if(rdCnt == 0)//EOF
		{
			return len - rdLen;//返回总共读到的字节数
		}

		rdLen -= rdCnt;
		ptr += rdCnt;
	}

	return len;//读到的字节数与准备读的字节数相等
}

int writen(int fd, void *buf, unsigned int len)
{
	size_t nLeft;
	ssize_t nWritten;
	char *ptr;
	ptr = buf;
	nLeft = len;

	while(nLeft > 0)
	{
		nWritten = write(fd, ptr, nLeft);
		if(nWritten <= 0) //返回0表示什么？
		{
			if(errno == EINTR)//如果写被信号中断，则继续写
			{
				continue;
			}
			else
			{
				return (-1);
			}
		}

		nLeft -= nWritten;
		ptr += nWritten;
	}

	return len;
}
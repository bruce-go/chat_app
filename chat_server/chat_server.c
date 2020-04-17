#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <mysql/mysql.h>

#include "../chat_app_with_qt4/client_server_comm.h"
#include "rdwrt_lib.h"

#define MAX_CLIENTNUM (FD_SETSIZE - 1)

char rd_buf[MAXLINE];
MYSQL *mysql_connection;

char *connfd_to_clientAccount[MAX_CLIENTNUM];

int process_client_request(int sockfd);

void user_register(int sockfd, Register_Msg_Data *msg);
void user_login(int sockfd, Login_Msg_Data *msg);
void add_friend_request(int sockfd, AddFriend_Msg_Data *msg);
void deliver_user_message_to_others(int sockfd, Chat_Msg_Data *msg);

int main(int argc, char *argv[])
{
	int listenfd, connfd, sockfd, maxfd, maxIdx, readyNum , i;
	int client_concted_fd[MAX_CLIENTNUM];
	fd_set rdSet, allSet;

	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	int rtnVal = 0;

	//1.初始化Mysql数据库
	mysql_connection = mysql_init(NULL);

	//1.1 连接到数据库, chatApp_db已经通过mysql命令行创建。如果连接到数据库失败，应该怎么处理？怎么判断连接失败？
	mysql_connection = mysql_real_connect(mysql_connection, argv[1], "root", "123456", "chatApp_db", 0, NULL, 0);

	//2.创建socket，并监听
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	//2.1给套接字绑定服务器地址
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	//2.2监听来自客户端的连接
	listen(listenfd, LISTENQ);

	// 分配一块空间用于保存读取的消息
	/*rdBuf = (char *)malloc(sizeof(Chat_Message));
    memset(rdBuf, 0, sizeof(Chat_Message));*/

	maxfd = listenfd;//初始化最大描述符为监听描述符
	maxIdx = -1;//初始化
	for(i = 0;i < MAX_CLIENTNUM; i++)
	{
		client_concted_fd[i] = -1;
		connfd_to_clientAccount[i] = NULL;
	}
	// printf("一个进程支持的最大文件描述符个数是 %d\n", MAX_CLIENTNUM);

	FD_ZERO(&allSet);
	FD_SET(listenfd, &allSet);

	for( ; ; )
	{
		rdSet = allSet;
		readyNum = select(maxfd+1, &rdSet, NULL, NULL, NULL);//阻塞等待满足可读条件的描述符

		if(FD_ISSET(listenfd, &rdSet))//如果监听套接字可读，表示有新的连接建立了
		{
			clilen = sizeof(cliaddr);
			connfd =  accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

			for(i = 0; i < MAX_CLIENTNUM; i++)//在client_conn数组中寻找空闲的位置来保存新的已连接描述符
			{
				if(client_concted_fd[i] == -1)
				{
					client_concted_fd[i] = connfd; //把新返回的已连接描述符保存起来
					break;
				}
			}

			if(i == MAX_CLIENTNUM)
			{
				fprintf(stderr, "%s\n", "Error, clients have reached the limit");
				exit(1);
			}

			FD_SET(connfd, &allSet);//把新描述符加入到集合中
			if(connfd > maxfd)//如果当前分配的描述符大于现有的最大描述符，则更新最大描述符
			{
				maxfd = connfd;
			}

			if(i > maxIdx)
			{
				maxIdx = i;//保存client_conn数组中当前使用的最大索引号
			}

			if(--readyNum <= 0)
			{
				continue;
			}
		}

		//处理每个已连接套接字
		for(i = 0; i <= maxIdx; i++)
		{
			if( (sockfd = client_concted_fd[i]) < 0)
			{
				continue;
			}

			if(FD_ISSET(sockfd, &rdSet))
			{
				if( (rtnVal = process_client_request(sockfd)) == 0)
				{
					close(sockfd);//由于对端客户端的写已经关闭，因此无法继续从这个客户端读到什么了，需要关闭该套接字
					FD_CLR(sockfd, &allSet);;
					client_concted_fd[i] = -1;//清除该套接字在数组中的记录
					//connfd_to_clientAccount[]
				}
				else if(rtnVal < 0)
				{
					exit(1);
				}

				if(--readyNum == 0)
				{
					break;
				}
			}
		}
	}
	/*for( ; ; )
	{
		clilen = sizeof(cliaddr);
		// printf("process %d start accept(), and sleep\n", getpid());
		//3.与客户建立连接后，如果收到客户端发来的消息，就根据操作码处理
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		// printf("process %d finished accept()\n", getpid());

		if((childpid = fork()) == 0)
		{
			close(listenfd);
			// printf("point 1, connect to sqlserver succeeds\n");
			//处理用户发来的消息
			// printf("process %d point 2, child start process\n", getpid());
again:
			if( (rtnVal = process_client_request(connfd)) < 0)
			{
				
				exit(1);
			}
			// printf("process %d point 5, child process finished\n", getpid());
			goto again;
		}
		//父进程中关闭已连接套接字，否则如果每个accept返回的已连接套接字都不调用close，那么父进程将耗尽可用描述符
		//还有就是当子进程关闭已连接套接字时，它的引用计数值将由2递减为1且保持为1，因为父进程永远不关闭任何已连接套接字，
		//这将妨碍TCP连接终止序列的发生，导致连接一直打开着
	
		close(connfd);
	}*/
	//关闭数据库，应该在关闭进程时的信号处理函数中调用
	mysql_close(mysql_connection);
}


int process_client_request(int sockfd)
{
	int rtnVal;
	MsgHead rcvdMsgHead;
    char rdBuf[sizeof(Chat_Message)];

    memset(&rcvdMsgHead, 0, sizeof(MsgHead));
    memset(rdBuf, 0, sizeof(Chat_Message));
	// General_Message *msg;
	
	//从TCP中读消息头
	if( (rtnVal = readOnce(sockfd, (void *)&rcvdMsgHead, sizeof(MsgHead))) < 0)//怎样判断读取完毕了？
	{
		fprintf(stderr, "process_client_request: read err\n");
		return -1;
	}
	else if(rtnVal == 0) //对端的写半部已经关闭，所以读到了EOF
	{
		return 0;
	}

	// printf("point 3, read from client succeeds\n");
	//处理读到的消息
	// msg = (General_Message *)rdBuf;//先转换成通用消息格式，以获取操作码
	switch(rcvdMsgHead.opCode)
	{
		case E_REGISTER:
			// printf("point 4, start register\n");
			rtnVal = readOnce(sockfd, (void *)rdBuf, rcvdMsgHead.dataLen);
			user_register(sockfd, (Register_Msg_Data *)rdBuf);//在知道具体操作类型后，再转换成具体的消息类型进一步处理
			break;
		case E_LOGIN:
			// printf("point 4, start login\n");
			rtnVal = readOnce(sockfd, (void *)rdBuf, rcvdMsgHead.dataLen);
			user_login(sockfd, (Login_Msg_Data *)rdBuf);
			break;
		case E_ADDFRIEND:
			printf("point 4, add friend\n");
			rtnVal = readOnce(sockfd, (void *)rdBuf, rcvdMsgHead.dataLen);
			add_friend_request(sockfd, (AddFriend_Msg_Data *)rdBuf);
			break;
		case E_CHATMSG:
			// printf("point 4, deliver msg to friend\n");
			rtnVal = readOnce(sockfd, (void *)rdBuf, rcvdMsgHead.dataLen);
			deliver_user_message_to_others(sockfd, (Chat_Msg_Data *)rdBuf);
			break;
		default:
			break;

	}

	return 1;
}


/*用户注册时应该先生成用户名（账号），传到客户端的注册界面，然后等用户填写密码*/
void user_register(int sockfd, Register_Msg_Data *msg)
{
	int writeNum;
	int res;
	Response_Message responseMsg;
	memset(&responseMsg, 0, sizeof(Response_Message));
	
	// 使用服务器生成的账号，发回客户端，让用户只需要设置密码即可，然后发到服务器保存
	//1. 把消息中的用户名和密码保存到数据库中
	char insert_cmd[200];
	sprintf(insert_cmd, "INSERT INTO namePswd(name,password) VALUES('%s','%s')", msg->name, msg->password);
	 // insert_cmd = "INSERT INTO namePswd VALUES ('"+(msg->namePasswd.name)+"', '"+(msg->namePasswd.password)+"')";
	fprintf(stdout, "register name = %s, pswd = %s\n", msg->name, msg->password);
	res = mysql_query(mysql_connection, insert_cmd);

	if(!res)
	{
		printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(mysql_connection));//打印受UPDATA、INSERT、DELETE查询影响的行数
		//2. 保存成功后，给客户端返回保存成功的标志，如果失败则给客户端传消息，显示服务器忙，
		responseMsg.msgHead.opCode = E_RESPONSE;
		responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

		responseMsg.respMsgData.operationStatus = E_TRUE;
		strcpy((char *)(responseMsg.respMsgData.response), "注册成功!");
		//3. 为注册的每个用户保存状态（离线；在线）
	}
	else
	{
		fprintf(stderr, "Insert error %d : %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));
		responseMsg.msgHead.opCode = E_RESPONSE;
		responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

		responseMsg.respMsgData.operationStatus = E_FALSE;
		strcpy((char *)(responseMsg.respMsgData.response), "服务器忙(或其他错误)，请稍后再试!");
	}
	

	// mysql_free_result(result);
	//4. 因为服务器与客户端之间除了网络问题或者服务器忙不过来这两种情况以外，其他状况都应该在服务器上解决掉，而不该反馈到客户端
	writeNum = writen(sockfd, (void *)&responseMsg, sizeof(Response_Message));
}



void user_login(int sockfd, Login_Msg_Data *msg)
{
	//1. 根据消息中的用户名，在数据库中查找其密码，与用户传过来的密码进行对比，如果一致，则给客户端返回登录成功标志
	//如果没有此用户或者密码不对，则返回相应消息给客户端
	int writeNum;
	Response_Message responseMsg;
	int res;
	MYSQL_RES *result;
	int numFields;
	MYSQL_ROW row;
	memset(&responseMsg, 0, sizeof(Response_Message));

	char query_cmd[100];
	
	// select 1 from tablename ;
	fprintf(stdout, "login name length= %d, pswd = %s\n", strlen(msg->name), msg->password);
	sprintf(query_cmd, "SELECT * FROM namePswd where name = '%s'", msg->name);
	// sprintf(query_cmd, "SELECT * FROM namePswd where password = '%s'", msg->namePasswd.password);
	 // insert_cmd = "INSERT INTO namePswd VALUES ('"+(msg->namePasswd.name)+"', '"+(msg->namePasswd.password)+"')";
	res = mysql_query(mysql_connection, query_cmd);//命令操作成功则返回0

	if(!res) //查询成功
	{
		//2. 提取所有找到的数据
		result = mysql_store_result(mysql_connection);
		if(result != NULL)
		{
			// printf("Found %lu rows\n", (unsigned long)mysql_num_rows(result));//返回查找到的记录的数目
			//3. 返回查找到的记录的数目不为0
			if( (unsigned long)mysql_num_rows(result) != 0)
			{
				//统计 table 含有的字段
				numFields = mysql_num_fields(result);
				printf("table has %d fields\n", numFields);
			 
			 	row = mysql_fetch_row(result);
			 	//if(row[1] == msg->password) //密码正确
			 	printf("row[1]=%s,\n", row[1]);
			 	printf("msg->namePasswd.password=%s,\n", msg->password);
				if(strcmp(row[1], msg->password) == 0)//查询到的密码与传过来的登录密码一致，则登录成功
				{
					responseMsg.msgHead.opCode = E_RESPONSE;
					responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

					responseMsg.respMsgData.operationStatus = E_TRUE;
					strcpy((char *)responseMsg.respMsgData.response, "登录成功");

					//在数组中保存以已连接描述符来表示的客户端与客户端账号之间的关系
					connfd_to_clientAccount[sockfd - 4] = malloc(strlen(msg->name) + 1);
					strcpy(connfd_to_clientAccount[sockfd - 4], msg->name);

					//4. 为每个用户创建一个数据库用来保存其好友列表等信息（将来可能还包含好友的个人信息）
					create_FriendList_Table_For_User(mysql_connection, msg);

				}
				else//如果能找到用户名，但是密码不对，则提示密码错误
				{
					responseMsg.msgHead.opCode = E_RESPONSE;
					responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

					responseMsg.respMsgData.operationStatus = E_FALSE;
					strcpy((char *)responseMsg.respMsgData.response, "密码错误!");
				}	
			}
			else
			{
				fprintf(stderr, "find error %d : %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));
				//如果找不到该用户，则提示没有此用户
				responseMsg.msgHead.opCode = E_RESPONSE;
				responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

				responseMsg.respMsgData.operationStatus = E_FALSE;
				strcpy((char *)responseMsg.respMsgData.response, "没有此用户，请检查你的账号!");
			}
		}
		else
		{
			printf("%s, Line %d: Error, mysql_store_result() return NULL \n", __FILE__, __LINE__);
		}

	}
	else//找不到该用户
	{
		fprintf(stderr, "find error %d : %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));
		//如果找不到该用户，则提示没有此用户
		// responseMsg.operationStatus = E_FALSE;
		// strcpy((char *)responseMsg.response, "没有此用户，请注册!");

	}
	
	mysql_free_result(result);

	writeNum = writen(sockfd, (void *)&responseMsg, sizeof(Response_Message));
}

void add_friend_request(int sockfd, AddFriend_Msg_Data *msg)
{
	//根据消息中的用户名，在数据库中查找，如果存在，则给客户端返回添加成功标志
	//如果没有此用户，则返回相应消息给客户端
	int writeNum;
	Response_Message responseMsg;
	int res;
	MYSQL_RES *result;
	int numFields;
	MYSQL_ROW row;
	memset(&responseMsg, 0, sizeof(Response_Message));

	char query_cmd[100];
	
	// fprintf(stdout, "friend id = %s\n", msg->currentUserName);
	sprintf(query_cmd, "SELECT * FROM namePswd where name = '%s'", msg->friendAccount);
	// sprintf(query_cmd, "SELECT * FROM namePswd where password = '%s'", msg->namePasswd.password);
	 // insert_cmd = "INSERT INTO namePswd VALUES ('"+(msg->namePasswd.name)+"', '"+(msg->namePasswd.password)+"')";
	res = mysql_query(mysql_connection, query_cmd);//命令操作成功则返回0

	if(!res) //查询成功
	{
		//提取所有找到的数据
		result = mysql_store_result(mysql_connection);
		if(result != NULL)//返回一个指向结果集结构的指针，失败返回NULL
		{
			if( (unsigned long)mysql_num_rows(result) != 0)//返回查找到的记录的数目为0
			{
				//构造响应消息
				responseMsg.msgHead.opCode = E_RESPONSE;
				responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

				responseMsg.respMsgData.operationStatus = E_TRUE;
				strcpy((char *)responseMsg.respMsgData.response, msg->friendAccount);//添加好友成功,则把好友账号返回，以便用户把好友添加到自己的列表中
				//把好友名字保存到每个用户各自的好友列表数据库中
				store_friend_to_users_list(mysql_connection, msg, &responseMsg);
			}	
			else
			{
				responseMsg.msgHead.opCode = E_RESPONSE;
				responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

				responseMsg.respMsgData.operationStatus = E_FALSE;
				strcpy((char *)responseMsg.respMsgData.response, "没有此用户，请检查你查找的账号!");
			}			
		}
		else
		{
			printf("%s, Line %d: Error, mysql_store_result() return NULL \n", __FILE__, __LINE__);
		}

	}
	else//查询过程中出错
	{
		fprintf(stderr, "find error %d : %s\n", mysql_errno(mysql_connection), mysql_error(mysql_connection));
		//如果找不到该用户，则提示没有此用户
		// responseMsg.operationStatus = E_FALSE;
		// strcpy((char *)responseMsg.response, "没有此用户，请检查你查找的账号!");

	}
	
	mysql_free_result(result);

	writeNum = writen(sockfd, (void *)&responseMsg, sizeof(Response_Message));
}

void deliver_user_message_to_others(int sockfd, Chat_Msg_Data *msg)
{
	//根据消息中的to字段，选择把消息转发给谁（需要维护一个关于已连接描述符和其客户端用户账号的数组，以便于查找）
	int friend_fd, i;
	int writeNum;
	Response_Message responseMsg;
	Chat_Message chatMsg;
	memset(&responseMsg, 0, sizeof(Response_Message));
	memset(&chatMsg, 0, sizeof(Chat_Message));

	for(i = 0; i < MAX_CLIENTNUM; i++)
	{
		if(connfd_to_clientAccount[i] == NULL)
		{
			continue;
		}

		if(strcmp(connfd_to_clientAccount[i], msg->to) == 0)
		{
			// printf("point 5, succeeds finding the friend to send to \n");
			/*****************把接收到的消息打印出来**********************/
			/*printf("chat msg server receved: \n");
			printf("from:%s\n", msg->from);
			printf("to:%s\n", msg->to);
			printf("message:%s\n", msg->message);*/
			/**********************************************************/
			friend_fd = i+4;
			//然后构造消息，
			chatMsg.msgHead.opCode = E_CHATMSG;
			chatMsg.msgHead.dataLen = sizeof(Chat_Msg_Data);

			memcpy((char *)&(chatMsg.chatMsgData), (char *)msg, sizeof(Chat_Msg_Data));

			/*****************把要发送的消息打印出来**********************/
			/*printf("chat msg server to send: \n");
			printf("from:%s\n", chatMsg.chatMsgData.from);
			printf("to:%s\n", chatMsg.chatMsgData.to);
			printf("message:%s\n", chatMsg.chatMsgData.message);*/
			/**********************************************************/
			writeNum = writen(friend_fd, (void *)&chatMsg, sizeof(Chat_Message));

			break;
		}
	
	}

	if(i == MAX_CLIENTNUM)//遍历以后没有找到，则给源端返回消息，提示对端不在线
	{
		responseMsg.msgHead.opCode = E_RESPONSE;
		responseMsg.msgHead.dataLen = sizeof(Response_Msg_Data);

		responseMsg.respMsgData.operationStatus = E_FALSE;
		strcpy( (char *)responseMsg.respMsgData.response, "好友未登录，无法发送消息！");
		writeNum = writen(sockfd, (void *)&responseMsg, sizeof(Response_Message));
	}
		
}


/*待完成！！！！！！！！！！*/
void create_FriendList_Table_For_User(MYSQL *mysql_connec, Login_Msg_Data *msg)
{
	int res_show, res_create;
	MYSQL_RES *result;
	char query_database_exist_cmd[100];
	char create_database_cmd[100];
	char friendListTableName[30];
	//创建之前先查询，如果该数据库已经存在，则不用再次创建
	sprintf(friendListTableName, "%s_FriendList", msg->name);
	sprintf(query_database_exist_cmd, "SHOW TABLES like '%s'", friendListTableName);

	res_show = mysql_query(mysql_connec, query_database_exist_cmd);
	if(!res_show) //查询成功
	{
		//2. 提取所有找到的数据
		result = mysql_store_result(mysql_connection);
		if(result != NULL)
		{
			if( (unsigned long)mysql_num_rows(result) == 0)//返回查找到的记录的数目为0
			{
				//4. 登录成功后，用一个函数为当前用户创建一个新的数据库，用来保存好友列表
				sprintf(create_database_cmd, "CREATE TABLE %s(account VARCHAR(30))", friendListTableName);
				res_create = mysql_query(mysql_connec, create_database_cmd);
				if(res_create)
				{
					fprintf(stderr, "find error %d : %s\n", mysql_errno(mysql_connec), mysql_error(mysql_connec));
				}
			}
			else//相应的表已经存在了,第二次及以后添加好友时
			{
				//直接把好友信息保存到表中
			}
		}
		else
		{
			printf("%s, Line %d: Error, mysql_store_result() return NULL \n", __FILE__, __LINE__);
		}
	}
	else
	{
		fprintf(stderr, "find error %d : %s\n", mysql_errno(mysql_connec), mysql_error(mysql_connec));
	}
}

void store_friend_to_users_list(MYSQL *mysql_connec, AddFriend_Msg_Data *msg, Response_Message *responseMsg)
{
	int res_insert;
	// char query_database_exist_cmd[100];
	char insert_cmd[100];
	char friendListTableName[30];
	//创建之前先查询，如果该数据库已经存在，则不用再次创建
	sprintf(friendListTableName, "%s_FriendList", msg->currentUserName);
	sprintf(insert_cmd, "INSERT INTO %s(account) VALUES('%s')", friendListTableName, msg->friendAccount);

	res_insert = mysql_query(mysql_connec, insert_cmd);
	if(!res_insert) //查询成功
	{
		// strcpy((char *)(responseMsg->respMsgData.response), "添加好友成功!");
	}
	else
	{
		fprintf(stderr, "find error %d : %s\n", mysql_errno(mysql_connec), mysql_error(mysql_connec));
	}
}






#ifndef CLIENTSERVERCOMM_H
#define CLIENTSERVERCOMM_H

#define SERV_PORT 9877
#define SERVER_IP_ADDRESS "127.0.0.1"
#define LISTENQ     1024    /* 2nd argument to listen() */

#define MAX_USER_NAME_LENGTH 12
#define MAX_USER_PSWD_LENGTH 20
#define SINGLE_MSG_LENGTH 1024
// #define RESPONSE_MSG_LENGTH 1024
#define MAXLINE 4096

typedef enum 
{
    E_FALSE = 0,
    E_TRUE
} BOOL;

typedef struct 
{
    char name[MAX_USER_NAME_LENGTH];
    char password[MAX_USER_PSWD_LENGTH];
} NamePasswordPair;

typedef enum 
{
    E_REGISTER = 0,
    E_LOGIN,
    E_ADDFRIEND,
    E_CHATMSG,
    E_RESPONSE
}Request_Opcode_Enum;

typedef struct 
{
    Request_Opcode_Enum opCode;
    unsigned int dataLen;
}MsgHead;

//建立客户端到服务器的消息格式，包括期望服务器对此消息作何操作，消息内容，响应等

typedef struct 
{
    Request_Opcode_Enum opCode;
    NamePasswordPair namePasswd;
    char response[SINGLE_MSG_LENGTH];
    char message[SINGLE_MSG_LENGTH];
}General_Message;

/************************注册消息类型***********************/
typedef struct 
{
    char name[MAX_USER_NAME_LENGTH];
    char password[MAX_USER_PSWD_LENGTH];
}Register_Msg_Data;

typedef struct 
{
    MsgHead msgHead;
    Register_Msg_Data regMsgData;
}Register_Message;

/*********************************************************/

/***********************登录消息类型***********************/
typedef struct 
{
    char name[MAX_USER_NAME_LENGTH];
    char password[MAX_USER_PSWD_LENGTH];
}Login_Msg_Data;

typedef struct 
{
    MsgHead msgHead;
    Login_Msg_Data loginMsgData;
}Login_Message;

/*********************************************************/

/***********************添加好友消息***********************/
typedef struct 
{
    char friendAccount[MAX_USER_NAME_LENGTH];
    char currentUserName[MAX_USER_NAME_LENGTH];
}AddFriend_Msg_Data;


typedef struct 
{
    MsgHead msgHead;
    AddFriend_Msg_Data addFrienMsgData;
}AddFriend_Message;

/*********************************************************/


/***********************聊天消息类型***********************/
typedef struct 
{
    char from[MAX_USER_NAME_LENGTH];
    char to[MAX_USER_NAME_LENGTH];
    char message[SINGLE_MSG_LENGTH];
}Chat_Msg_Data;

typedef struct 
{
    MsgHead msgHead;
    Chat_Msg_Data chatMsgData;
}Chat_Message;

/*********************************************************/


//
/***********************服务器对客户端的响应消息格式**********/
typedef struct 
{
    char operationStatus;
    char response[SINGLE_MSG_LENGTH];
}Response_Msg_Data;

typedef struct 
{
    MsgHead msgHead;
    Response_Msg_Data respMsgData;
} Response_Message;

/*********************************************************/

#endif 
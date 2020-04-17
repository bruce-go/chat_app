#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <pthread.h>

#include "loginDialog.h"
#include "registerDialog.h"
#include "addFriendDialog.h"
#include "chatDialog.h"
#include "socketCommunication.h"

#define TMP_STRING_LEN   125
#define MAXCHATDIALOGNUM 5

class QAction;
class QLabel;
class QLineEdit;
class FindDialog;
class QPushButton;
// class AddCdDialog;
// class AddCdTrack;
class QTreeWidget;
class QTreeWidgetItem;
class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;
class QStringList;
// class QList<QTreeWidgetItem *>;
class QTreeWidgetItem;


class ChatMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatMainWindow();

    /*void startThread()
    {
        pthread_t tid;
        typedef void * (*pThreadFun)(void *);
        // pThreadFun callBack = (pThreadFun)&rcvAndDispatchChatMsg_thread;
        pthread_create(&tid, NULL, rcvAndDispatchChatMsg_thread , (void *)this);
    }*/

    void rcvAndDispatchChatMsg_thread();
    
protected:
    // void closeEvent(QCloseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);//继承自QWidget的虚函数，在子类中重新实现此函数即可用于处理上下文菜单事件

signals:
    void rcvAddFriendResponse_signal(bool operationStatus, QString responMsg);

    void dispatchMsgToChatDialog_signal(unsigned int idx, QString friendAccount, QString msg);


public slots:
    void friendRightClickedOperation();//(QTreeWidgetItem *, int);
    

private slots:
    void register_action_slot();
    void logIn_action_slot();
    void addFriend_action_slot();

    void about();
    void goThroughDatabase();
    // void enableSearchButton(const QString &text);
    void search_slot();
    
    void logIn_button_slot();
    void register_button_slot();
    void addFriend_button_slot();

    // void openRecentFile();
    // void updateStatusBar();
    
    void addCdInfoToTree();
    void changeCdCatalogItem();
    void changeCdTrackItem();
    
    void AddTracktoCd();
    void sendMsgToFriend_slot();
    void addFriendResponse_slot(bool operationStatus, QString responMsg);

    void renameFriend_slot();
    void deleteFriend_slot();
    void getMsgAndSendToFriend_slot(unsigned int idx, QString friendAccount);
    void rcvMsgFromFriend_slot(unsigned int idx, QString friendAccount, QString msg);
    // void rcvMsgFromFriend(unsigned int idx, QString friendAccount);

private:
    void createActions();
    void createMenus();
    // void createContextMenu();
    void createToolBars();
    // void createStatusBar();

    void showFriendList();

    void convertQstringtochar(char * dest, QString *srcDataa);

    //用户登录后，从服务器获取当前用户的好友列表
    void get_Friendlist_from_Database();

    //把好友添加到用户的好友列表中
    void addFriendToMyList(QString friendAccount);
    // void rcvAndDispatchChatMsg();
    // void readFromServer();
    // void writeToServer();

    QWidget *mainWidget;
    
    RegisterDialog *registerDialog;
    LoginDialog *loginDialog;
    AddFriendDialog *addFriendDialog;
    ChatDialog *chatDialog[MAXCHATDIALOGNUM];
    QString chatDialogFriendAccount[MAXCHATDIALOGNUM];//保存每个对话框的聊天对象
    unsigned int currentChatDialogIdx = 0;

    QLineEdit *lineEdit_Search;

    SocketCommunication *sockCommu;
    int sockfd = 0xFFFFFFFF;
    int testVar = 2;

    QString userName; //当前登录的用户
    QString passWord;
    bool connectStatus = false;
    bool logInStatus = false;
    pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu * contextMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *register_Action;
    QAction *logIn_Action;
    QAction *addFriend_Action;
    QAction *quit_Action;
    QAction *deleteAction;
    QAction *update_Action;
    QAction *aboutAction;
    QAction *aboutQtAction;

    QHBoxLayout *topHorizontalLayout;
    QGridLayout *cdTreeLayout;
    QVBoxLayout *mainLayout;

    QTreeWidget *treeWidget;
    QStringList treeWidgetLabelList;

    QList<QTreeWidgetItem *> friendList_WidgetItem_QList;
    // QList<QTreeWidgetItem *> cd_Item_List1;
    QTreeWidgetItem *friendDir_WidgetItem;

    QTreeWidgetItem *friend_WidgetItem;

    QTreeWidgetItem *current_Item;

    int text_track_num;
};




#endif

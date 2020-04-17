#include <QtGui>
#include <iostream>

#include <string>

// #include "finddialog.h"
// #include "addCDdialog.h"
#include "chatMainWindow.h"
#include "client_server_comm.h"
#include <stdio.h>
// #include "addCDtrack.h"

extern QApplication *app;

ChatMainWindow::ChatMainWindow()
{
    //创建通信对象
    sockCommu = new SocketCommunication();

    createActions();
    createMenus();//创建菜单栏
    createToolBars();//创建工具栏
    //状态栏待创建
    this->resize(350,700);
    
    mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    mainWidget->setMouseTracking(true);

    lineEdit_Search = new QLineEdit;

    connect(lineEdit_Search, SIGNAL(returnPressed()), this, SLOT(search_slot()));

    //初始化保存聊天对话框的指针数组,和某个对话框的聊天对象
    for(int i = 0; i < MAXCHATDIALOGNUM; i++)
    {
        chatDialog[i] = NULL;
        chatDialogFriendAccount[i] = QString("");
    }
    
    showFriendList();

    // createStatusBar();
    // findDialog = 0;

    setWindowIcon(QIcon(":/images/icon.jpg"));
    setWindowTitle(tr("Chat app"));
}




void ChatMainWindow::showFriendList()
{
    treeWidgetLabelList << tr("All  Friends") ;

    // cdTreeLayout = new QGridLayout;
    QHBoxLayout *friendListLayout = new QHBoxLayout();

    treeWidget = new QTreeWidget();
    //添加树形列表
    // treeWidget = new QTreeWidget();
    // treeWidget->setColumnCount(4);
    // treeWidget->setSortingEnabled(true);
    treeWidget->setHeaderLabels(treeWidgetLabelList);
    // treeWidget->setGeometry(0,0, this->width(), this->height());

    //遍历数据库
    // goThroughDatabase();
    get_Friendlist_from_Database();

    // treeWidget->insertTopLevelItems(0, friendList_WidgetItem_QList);

    // connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(friendRightClickedOperation(QTreeWidgetItem *, int)));

    // cdTreeLayout->addWidget(treeWidget, 0, 0);
     // friendListLayout->

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(lineEdit_Search);
    mainLayout->addWidget(treeWidget);

    mainWidget->setLayout(mainLayout);
}

void ChatMainWindow::get_Friendlist_from_Database()
{
    //从数据库读取本用户的好友列表
}

void ChatMainWindow::goThroughDatabase()
{
    /*treeWidget->clear();
    //在启动过程中可以读取数据库，如果数据库非空则将已保存的CD信息显示出来
    //是否有函数可以直接遍历DBM数据库？怎样判断dbm数据库为空？
    // 两层循环，第一层遍历目录，第二层遍历目录下的track
    cd_catalog_entry catalog_Item_In_DBM;
    cd_track_entry   existing_track_In_DBM;
    
    //dbm数据库的第一个键应该是指向最后一个加入的对象，或者数据库初始化时有很多key_datum.dptr == NULL的对象
    // dbm_nextkey()遍历到的键值包含的指针为空后，继续往后获取的键值包含的指针也会依然为空
    for(catalog_Item_In_DBM = cd_catalog_DBM_FirstItem();catalog_Item_In_DBM.catalog_entry[0] != '\0';catalog_Item_In_DBM = cd_catalog_DBM_NextItem())
    {
        //先把目录条目的成员转换成QString，然后添加到treeWidget中
        text_Catalog_entry = QString(QLatin1String(catalog_Item_In_DBM.catalog_entry));
        text_Title         = QString(QLatin1String(catalog_Item_In_DBM.title));
        text_Type          = QString(QLatin1String(catalog_Item_In_DBM.type));
        text_Artist        = QString(QLatin1String(catalog_Item_In_DBM.artist));

        friend_WidgetItem = new QTreeWidgetItem(treeWidget, QStringList()<< text_Catalog_entry
                            << text_Title << text_Artist << text_Type);
        //设置QTreeWidgetItem的各列成员的颜色，字体等
         QBrush brush_catalog = QBrush(QColor(199, 237, 225));
         friend_WidgetItem->setBackground(0, brush_catalog);
         friend_WidgetItem->setBackground(1, brush_catalog);
         friend_WidgetItem->setBackground(2, brush_catalog);
         friend_WidgetItem->setBackground(3, brush_catalog);

        //再遍历目录下的所有track，并添加到catalog_WidgetItem中
        int track_no = 1;
        while(1)
        {
            // std::cout << "run to catalog " << catalog_Item_In_DBM.catalog_entry[0] << " item "  << track_no << std::endl;
            existing_track_In_DBM = get_cd_track_entry((const char *)(&catalog_Item_In_DBM.catalog_entry), track_no);

            if(existing_track_In_DBM.catalog_entry[0] != '\0')//如果条目号存在且非0
            {

                track_WidgetItem = new QTreeWidgetItem(friend_WidgetItem, QStringList() << tr("Track")+QString::number(existing_track_In_DBM.track_no) 
                    << QString(QLatin1String(existing_track_In_DBM.track_txt)));

            }
            else
            {
                // std::cout << "catalog " << catalog_Item_In_DBM.catalog_entry[0] << " item " 
                //             << track_no  << " didn't not exist" << std::endl;
                // std:cout << std::endl;
                break;
            }

            track_no++;
        }
        
        treeWidget->expandItem(friend_WidgetItem);//把这个目录下的子项设为展开
        friendList_WidgetItem_QList << friend_WidgetItem;
    }*/
}


void ChatMainWindow::addCdInfoToTree()
{
    /*text_Catalog_entry = addCdDialog->getCatalogEntry();
    text_Title         = addCdDialog->getCdTitle();
    text_Type          = addCdDialog->getCdType();
    text_Artist        = addCdDialog->getCdArtist();

    friend_WidgetItem = new QTreeWidgetItem(treeWidget, QStringList()<< text_Catalog_entry
    << text_Title << text_Artist << text_Type);

    //设置QTreeWidgetItem的各列成员的颜色，字体等
    QBrush brush_catalog = QBrush(QColor(199, 237, 225));//199, 237, 204
    friend_WidgetItem->setBackground(0, brush_catalog);
    friend_WidgetItem->setBackground(1, brush_catalog);
    friend_WidgetItem->setBackground(2, brush_catalog);
    friend_WidgetItem->setBackground(3, brush_catalog);

    friendList_WidgetItem_QList << friend_WidgetItem;

    // connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(friendRightClickedOperation(QTreeWidgetItem *, int)));

    treeWidget->insertTopLevelItems(0, friendList_WidgetItem_QList);
   
    //把目录信息保存到数据库中
    convertQstringtochar(cdCatalogEntry.catalog_entry, &text_Catalog_entry);
    convertQstringtochar(cdCatalogEntry.title, &text_Title);
    convertQstringtochar(cdCatalogEntry.type, &text_Type);
    convertQstringtochar(cdCatalogEntry.artist, &text_Artist);

    // std::cerr << "run to 4!" << std::endl;
    if(!add_cd_catalog_entry(cdCatalogEntry))
    {
        //dbm保存失败了，打印错误并返回
        std::cerr << "Error, dbm save failed!" << std::endl;
    }*/
    
}


void ChatMainWindow::changeCdCatalogItem()
{
    /*text_Catalog_entry = addCdDialog->getCatalogEntry();
    text_Title         = addCdDialog->getCdTitle();
    text_Artist        = addCdDialog->getCdArtist();
    text_Type          = addCdDialog->getCdType();

    current_Item = treeWidget->currentItem();

    current_Item->setText(1, text_Title);
    current_Item->setText(2, text_Artist);
    current_Item->setText(3, text_Type);

     memset(&cdCatalogEntry, '\0', sizeof(cdCatalogEntry));
    //把目录信息保存到数据库中
    convertQstringtochar(cdCatalogEntry.catalog_entry, &text_Catalog_entry);
    convertQstringtochar(cdCatalogEntry.title, &text_Title);
    convertQstringtochar(cdCatalogEntry.type, &text_Type);
    convertQstringtochar(cdCatalogEntry.artist, &text_Artist);

    // std::cerr << "run to 4!" << std::endl;
    if(!add_cd_catalog_entry(cdCatalogEntry))
    {
        //dbm保存失败了，打印错误并返回
        std::cerr << "Error, dbm save failed!" << std::endl;
    }*/
    
}


void ChatMainWindow::convertQstringtochar(char * dest, QString *srcData)
{
    //srcData->toStdString()返回的是一个string对象，并存放在栈中，不能直接把该对象转换成char *的指针赋值给别的指针变量
    strcpy(dest, srcData->toStdString().c_str());
    // dest = (char *)(srcData->toStdString().c_str());
    //或者先将QString用toStdString()转为标准库中的string类型，然后将string用c_str()转为char *
}

void ChatMainWindow::friendRightClickedOperation()//(QTreeWidgetItem *, int)
{
    current_Item = treeWidget->currentItem();

    if(current_Item == NULL)  //如果没有选中树上的任何项目，则直接返回
    {
        return;
    }
   
    QAction *sendMsg_Action = new QAction(tr("send message"), this);
    QAction *renameFriend_Action = new QAction(tr("Rename"), this);
    QAction *delFriend_Action = new QAction(tr("Delete"), this);

    connect(sendMsg_Action, SIGNAL(triggered()), this, SLOT(sendMsgToFriend_slot()));
    connect(renameFriend_Action, SIGNAL(triggered()), this, SLOT(renameFriend_slot()));//这个槽函数待实现
    connect(delFriend_Action, SIGNAL(triggered()), this, SLOT(deleteFriend_slot()));//这个槽函数待实现
    
    QPoint pos;
    contextMenu = new QMenu(treeWidget);
    /*if(current_Item->parent() != NULL) //目录项才允许添加track信息，如果是track项则只能重命名和删除
    {
        // menu.addAction(addTrack_Action);
        contextMenu->addAction(renameCdItem_Action);
        contextMenu->addAction(delCdItem_Action);
        contextMenu->exec(QCursor::pos());  //在当前鼠标位置显示
    }
    else
    {*/
        contextMenu->addAction(sendMsg_Action);
        contextMenu->addAction(renameFriend_Action);
        contextMenu->addAction(delFriend_Action);
        contextMenu->exec(QCursor::pos());  //在当前鼠标位置显示
    /*}
    else if(qApp->mouseButtons() == Qt::)处理双击事件*/
}

void ChatMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    friendRightClickedOperation();
}

void ChatMainWindow::AddTracktoCd()
{
   /* QTreeWidgetItem *current_Item = treeWidget->currentItem();

    if(current_Item == NULL)  //如果没有选中树上的任何项目，则直接返回
    {
        return;
    }

    addCdTrackDialog = new AddCdTrack(this);

    connect(addCdTrackDialog, SIGNAL(_add_clicked_signal()),this, SLOT(addTrackItem()));

    addCdTrackDialog->show();
    
    addCdTrackDialog->raise();
   
    addCdTrackDialog->activateWindow();*/
}

//起一个单独的线程负责阻塞并等待套接字上的消息可读，然后分发给各个对话框
//只起一个线程，要是打开了新的对话框
//要是多个线程读一个套接字，那么消息应该只会被某一个线程读走，且读走后消息就从套接字缓冲区中被删除了。
/*void *ChatMainWindow::rcvAndDispatchChatMsg_thread(void *arg)
{
    
    ChatMainWindow *curMainWindow = (ChatMainWindow *)arg;
    // for( ; ; )''
    // std::cout << "thread is running" << std::endl;
    // std::cout << "curMainWindow->testVar = " << curMainWindow->testVar << std::endl;
    // std::cout << "curMainWindow->logInStatus = " << curMainWindow->logInStatus << std::endl;
    // while(curMainWindow->logInStatus == true)
    curMainWindow->rcvAndDispatchChatMsg();
    
    
    //获取该字段对应的对话框句柄索引号，

    //发送信号
    // emit dispatchMsgToChatDialog_signal(idx, friendAccount);
}*/

//线程函数中不能处理UI相关的组件（QWidget子类）
void ChatMainWindow::rcvAndDispatchChatMsg_thread()
{
    //阻塞，从套接字接收消息

    MsgHead rcvdMsgHead;
    char *rdBuf;

    std::cout << "thread is running" << std::endl;

    memset(&rcvdMsgHead, 0, sizeof(MsgHead));
    rdBuf = (char *)malloc(sizeof(Chat_Message));
    memset(rdBuf, 0, sizeof(Chat_Message));

    unsigned int cnt = 0;
    for( ; ; )
    {
        cnt++;
        // pthread_mutex_lock(&counter_mutex);
        if((logInStatus == false))
        // pthread_mutex_unlock(&counter_mutex);
        {
            if(cnt%3000000000 == 0)
            {
                std::cout << "still continue..." << std::endl;
            }
            continue;
        }
        //从服务器读取响应
        std::cout << "start read MsgHead from server" << std::endl;
        sockCommu->readFromServer(sockfd, (void *)&rcvdMsgHead, sizeof(MsgHead));
        //如果是响应消息
        if(rcvdMsgHead.opCode == E_RESPONSE)
        {
            //根据消息头中指定的数据长度来读取剩余的消息数据部分
            sockCommu->readFromServer(sockfd, (void *)rdBuf, rcvdMsgHead.dataLen);

            emit rcvAddFriendResponse_signal(((Response_Msg_Data *)rdBuf)->operationStatus, 
                                             QString::fromLocal8Bit( ((Response_Msg_Data *)rdBuf)->response));
        }

        //如果是聊天消息
        if(rcvdMsgHead.opCode == E_CHATMSG)
        {
            //根据消息头中指定的数据长度来读取剩余的消息数据部分
            sockCommu->readFromServer(sockfd, (void *)rdBuf, rcvdMsgHead.dataLen);

            std::cout << "read chat Msg from server finished" << std::endl;
            printf("from:%s\n", ((Chat_Msg_Data *)rdBuf)->from);
            printf("to:%s\n", ((Chat_Msg_Data *)rdBuf)->to);
            printf("msg:%s\n", ((Chat_Msg_Data *)rdBuf)->message);
            /*可以根据收到的消息的操作码类型先进行区分，如果是添加好友请求的回应消息，则进行相应处理
            *如果是好友聊天消息，则把消息转发到各个对话框*/

            //根据from字段判断该消息是发往哪个对话框的    
            // QString from;
            char from[12];
            memset(from, 0, sizeof(from));
            for(unsigned int idx = 0; idx < MAXCHATDIALOGNUM; idx++)
            {
                // from = QString::fromLocal8Bit(rcvdChatMsg.from);
                convertQstringtochar(from, &(chatDialogFriendAccount[idx]));
                std::cout << "rcvd msg from friends" << std::endl;
                if( strcmp(from, ((Chat_Msg_Data *)rdBuf)->from) == 0)
                {
                    std::cout << "chat msg from %s" << from << " ;it says:" << ((Chat_Msg_Data *)rdBuf)->message << std::endl;
                    /*chatDialog[idx]->appendMessage(QString::fromLocal8Bit( ((Chat_Msg_Data *)rdBuf)->from), 
                                                   QString::fromLocal8Bit( ((Chat_Msg_Data *)rdBuf)->message));*/
                    emit dispatchMsgToChatDialog_signal(idx, QString::fromLocal8Bit( ((Chat_Msg_Data *)rdBuf)->from),
                                                        QString::fromLocal8Bit( ((Chat_Msg_Data *)rdBuf)->message));
                }
            }
        }
        
    }
}
void ChatMainWindow::rcvMsgFromFriend_slot(unsigned int idx, QString friendAccount, QString msg)
{
    
    chatDialog[idx]->appendMessage(friendAccount, msg);
}

void ChatMainWindow::sendMsgToFriend_slot()
{
    QTreeWidgetItem *current_Item = treeWidget->currentItem();

    treeWidget->expandItem(current_Item);
    // std::cout << "current catalog items' child num :  " <<  << std::endl;
    // childNum = current_Item->childCount();
    //下一条语句执行以后current_Item的child数目就变成1了，后面再使用current_Item->childCount()获取到的值是1！！！！！
    /*track_WidgetItem = new QTreeWidgetItem(current_Item, 
        QStringList() << tr("Track")+QString::number(childNum + 1) << addCdTrackDialog->getTrackName());*/

    //
    QString friendAcnt = current_Item->text(0);

    //打开的对话框不能超过最大对话框数目，可扩展
    unsigned int tempChatDialogIdx;
    for(tempChatDialogIdx = 0; tempChatDialogIdx < MAXCHATDIALOGNUM; tempChatDialogIdx++)
    {
        if(chatDialog[tempChatDialogIdx] == NULL)
        {
            chatDialog[tempChatDialogIdx] = new ChatDialog(userName,friendAcnt, this);
            chatDialogFriendAccount[tempChatDialogIdx] = friendAcnt;
            //每个对话框保存自身的索引号
            chatDialog[tempChatDialogIdx]->setCurrentDialogInx(tempChatDialogIdx);
            //每个对话框保存聊天好友的账号
            // chatDialog[tempChatDialogIdx]->saveFriendChattingWith(friendAcnt);
            
            connect(chatDialog[tempChatDialogIdx], SIGNAL(sendMessageToFriend_signal(unsigned int, QString)), 
                this, SLOT(getMsgAndSendToFriend_slot(unsigned int, QString)));

            connect(this, SIGNAL(dispatchMsgToChatDialog_signal(unsigned int, QString, QString)), 
                this, SLOT(rcvMsgFromFriend_slot(unsigned int, QString, QString)) );

            chatDialog[tempChatDialogIdx]->setModal(false);
            chatDialog[tempChatDialogIdx]->show();
            break;
        }
    }

    if(tempChatDialogIdx == MAXCHATDIALOGNUM)
    {
        //提示达到了可以打开的最大对话框数目

        QMessageBox msgBox;

        msgBox.setText(QString::fromLocal8Bit("You'v reached the maximum chat dialog, and you can't open more!"));
        msgBox.exec();

    }
}

void ChatMainWindow::getMsgAndSendToFriend_slot(unsigned int idx, QString friendAccount)
{
    QString msgSentToFriend = chatDialog[idx]->getMessage();
    //把消息转发到服务器
    Chat_Message chatMsg;
    //Response_Message msgRead;
    //构造聊天消息
    memset(&chatMsg, 0, sizeof(Chat_Message));

    chatMsg.msgHead.opCode = E_CHATMSG;
    chatMsg.msgHead.dataLen = sizeof(Chat_Msg_Data);

    convertQstringtochar(chatMsg.chatMsgData.from, &userName);
    convertQstringtochar(chatMsg.chatMsgData.to, &friendAccount);
    convertQstringtochar(chatMsg.chatMsgData.message, &msgSentToFriend);
    //然后把用户名和密码上传到服务器进行查询校验，在服务器的数据库中根据用户名查询密码
    sockCommu->writeToServer(sockfd, (void *)&chatMsg, sizeof(Chat_Message));
    std::cout << "send chatMsg to server finished" << std::endl;
}

void ChatMainWindow::renameFriend_slot()
{
    /*QTreeWidgetItem *current_Item = treeWidget->currentItem();
    
    //char cur_catalog_entry_num[31];
    // int deleted_track_no, to_be_done_num;
    // cd_track_entry temp_track_In_DBM;

    //修改目录项
    if(current_Item->parent() == NULL)
    {
        //修改目录信息
        changeCdCatalogDialog = new AddCdDialog(this, current_Item->text(0), current_Item->text(1),
            current_Item->text(2), current_Item->text(3));
        connect(changeCdCatalogDialog, SIGNAL(_ok_clicked_signal()),this, SLOT(changeCdCatalogItem()));
        
        changeCdCatalogDialog->show();
        changeCdCatalogDialog->raise(); 
        changeCdCatalogDialog->activateWindow(); 
    }
    else// 修改track项
    {
        // QTreeWidgetItem *current_Item_Parent = current_Item->parent();

        changeCdTrackDialog = new AddCdTrack(this, /*current_Item->text(0).section('k', 1, -1) + current_Item->text(1));

        connect(changeCdTrackDialog, SIGNAL(_ok_clicked_signal()),this, SLOT(changeCdTrackItem()));

        changeCdTrackDialog->show();
        changeCdTrackDialog->raise();
        changeCdTrackDialog->activateWindow();
    }*/
}

void ChatMainWindow::deleteFriend_slot()
{
    /*QTreeWidgetItem *current_Item = treeWidget->currentItem();
    QString qstring_Current_Item;
    char cur_catalog_entry_num[31];
    int deleted_track_no, to_be_done_num;
    cd_track_entry temp_track_In_DBM;

    //删除目录项
    if(current_Item->parent() == NULL)
    {
        if(current_Item->childCount() > 0)//有子节点
        {
            for(int i = 0; i < current_Item->childCount(); i++)
            {
                //删除各个track项
                qstring_Current_Item = current_Item->text(0);
                convertQstringtochar(cur_catalog_entry_num, &qstring_Current_Item);;//获取column 0的内容

                deleted_track_no = std::stoi(current_Item->child(i)->text(0).section('k', 1, -1).toStdString());

                del_cd_track_entry(cur_catalog_entry_num, deleted_track_no);

                delete (current_Item->child(i));
            }
        }

        //删除目录
        qstring_Current_Item = current_Item->text(0);
        convertQstringtochar(cur_catalog_entry_num, &qstring_Current_Item);;//获取目录项column 0的内容
        
        if(del_cd_catalog_entry(cur_catalog_entry_num) != 1)
        {
            std::cout << "Error, delete catlog failed" << std::endl;
        }
        // delete treeWidget->takeTopLevelItem(treeWidget->currentIndex().row());
        delete current_Item;
    }
    else// 删除track项
    {
        QTreeWidgetItem *current_Item_Parent = current_Item->parent();

        //从树上删除该track项
        current_Item_Parent->removeChild(current_Item); 

        //从数据库中删除该track项
        qstring_Current_Item = current_Item_Parent->text(0);
        convertQstringtochar(cur_catalog_entry_num, &qstring_Current_Item);;//获取目录项column 0的内容
        deleted_track_no = std::stoi(current_Item->text(0).section('k', 1, -1).toStdString());   //只能处理个位数的track_no，扩展性需要进一步完善

        del_cd_track_entry(cur_catalog_entry_num, deleted_track_no);

        //检测并更新树的子项和数据库中的track项的track_no值
        // 遍历剩余子项，更新编号大于当前被删除子项编号的项，并写到数据库中
        int temp_track_no;
        int new_temp_track_no;
        int accumulation = 1;
        int adjust_finished_num = 0;
        to_be_done_num = current_Item_Parent->childCount() - (deleted_track_no - 1);//删除一个后目前剩余的子项目数，减去不需要更新的项目数
        while(1)//从被删除后的下一个项开始按顺序更新
        {
            
            for(int i = 0; i < current_Item_Parent->childCount(); i++)
            {
                temp_track_no = std::stoi(current_Item_Parent->child(i)->text(0).section('k', 1, -1).toStdString());
                
                if(temp_track_no == deleted_track_no + accumulation)
                {
                    new_temp_track_no = temp_track_no - 1;
                    current_Item_Parent->child(i)->setText(0, tr("Track")+QString::number(new_temp_track_no));
                    //由于键值更新了，要先在数据库中找出原来的track项，更新track_no，再删除原来的项，并利用新的键值重新存储
                    temp_track_In_DBM = get_cd_track_entry((const char *)cur_catalog_entry_num, temp_track_no);
                    temp_track_In_DBM.track_no = new_temp_track_no;
                    del_cd_track_entry(cur_catalog_entry_num, temp_track_no);

                    add_cd_track_entry(temp_track_In_DBM);

                    adjust_finished_num++;
                }
            }

            if(adjust_finished_num == to_be_done_num)
            {
                break;
            }

            accumulation++;

        }
        
        delete current_Item;
    }*/
}



void ChatMainWindow::changeCdTrackItem()
{
   /* QString current_Item_catalog_entry;

    QTreeWidgetItem *current_Item = treeWidget->currentItem();

    memset(&cdTrackEntry, '\0', sizeof(cdTrackEntry));
    
    //把音乐信息更新到数据库
    text_track_num = std::stoi(current_Item->text(0).section('k', 1, -1).toStdString());//addCdTrackDialog->getTrackNum();
    text_track_name = changeCdTrackDialog->getTrackName();
    current_Item_catalog_entry = current_Item->parent()->text(0);

    //修改条目中的track名字
    current_Item->setText(1, text_track_name);

    convertQstringtochar(cdTrackEntry.catalog_entry, &current_Item_catalog_entry);
   
    convertQstringtochar(cdTrackEntry.track_txt, &text_track_name);

    cdTrackEntry.track_no = text_track_num;//std::stoi(text_track_num.toStdString());
    // std::cout << "add track item " << text_track_num << std::endl;
    if(add_cd_track_entry(cdTrackEntry) != 1)
    {
        //dbm保存失败了，打印错误并返回
        std::cerr << "Error, dbm save failed!" << std::endl;
    }*/
}

void ChatMainWindow::search_slot()
{
    lineEdit_Search->clear();
    std::cout << "search lineEdit, return pressed!" << std::endl;
    /*int entry_selected =0;
    char tmp_str[TMP_STRING_LEN +1];
    int any_entry_found =0;
    int first_call = 1;
    cd_catalog_entry item_found;

    QString text_search = lineEdit_Search->text();

    convertQstringtochar(tmp_str, &text_search);

    while(!entry_selected)//需要改成跟启动时遍历一样，才能查找相同名字的多个目录
    {
        item_found = search_cd_catalog_entry(tmp_str, &first_call);
        if(item_found.catalog_entry[0] != '\0')
        {
            ++any_entry_found;//统计查找到的次数

            updateSearchedCatalogToTree(&item_found, any_entry_found);
            //找到了项目，设置标记
            // entry_selected = 1;
            // std::cout << any_entry_found <<" times found " << std::endl;
        }
        else
        {
            if(any_entry_found)
            {
                std::cout << "Sorry, no more matches found" << std::endl;
            }
            else
            {
                std::cout << "Sorry, nothing found" << std::endl;
            }
            break;
        }
    }*/
}



void ChatMainWindow::logIn_action_slot()
{
    loginDialog = new LoginDialog();

    connect(loginDialog, SIGNAL(_login_button_clicked_signal()), this, SLOT(logIn_button_slot()));

    loginDialog->exec();
}

void ChatMainWindow::logIn_button_slot()
{
    if(connectStatus == false)//还未与服务器进行连接，需要连接
    {
        sockfd = sockCommu->socket_connect();
        if(sockfd == -1)
        {
            //连接失败了，显示失败消息即可
            return;
        } 

        connectStatus = true;
    }

    //获取输入的用户名和密码
    userName = loginDialog->getLogInUserName();
    passWord = loginDialog->getLogInPassWord();

    Login_Message loginMsg;
    Response_Message responMsg;
    //构造登录消息
    memset(&loginMsg, 0, sizeof(loginMsg));
    memset(&responMsg, 0, sizeof(responMsg));

    loginMsg.msgHead.opCode = E_LOGIN;
    loginMsg.msgHead.dataLen = sizeof(Login_Msg_Data);

    convertQstringtochar(loginMsg.loginMsgData.name, &userName);
    convertQstringtochar(loginMsg.loginMsgData.password, &passWord);
    //然后把用户名和密码上传到服务器进行查询校验，在服务器的数据库中根据用户名查询密码
    sockCommu->writeToServer(sockfd, (void *)&loginMsg, sizeof(Login_Message));
    std::cout << "write to server finished" << std::endl;
    
    //从服务器读取响应
    sockCommu->readFromServer(sockfd, (void *)&responMsg, sizeof(Response_Message));
    std::cout << "read from server finished" << std::endl;
    QMessageBox msgBox;

    if(responMsg.respMsgData.operationStatus == false)
    {
        msgBox.setText(QString::fromLocal8Bit(responMsg.respMsgData.response));
        msgBox.exec();
    }
    else
    {
        //如果用户名和密码均正确，则则关闭登录对话框，显示主窗口
        loginDialog->close();
        // 当前用户登录成功后，不允许在同一个客户端上登录多个用户，必须重新打开一个客户端。
        // 登录成功以后，需要把登录按钮和注册按钮无效掉，
        register_Action->setEnabled(false);
        logIn_Action->setEnabled(false);

        // pthread_mutex_lock(&counter_mutex);
        logInStatus = true;
        // pthread_mutex_unlock(&counter_mutex);
        // 然后重新connect一个函数，函数里在点击登录选项时弹出消息框显示"当前用户已登录

        //并更新当前用户的好友列表，消息历史记录，头像等
    }    
}

void ChatMainWindow::register_action_slot()
{
    registerDialog = new RegisterDialog();

    connect(registerDialog, SIGNAL(_register_confirmed_signal()), this, SLOT(register_button_slot()));

    registerDialog->exec();
}

void ChatMainWindow::register_button_slot()
{
    //先与服务器建立连接
    if(connectStatus == false)//还未与服务器进行连接，需要连接
    {
        sockfd = sockCommu->socket_connect();
        if(sockfd == -1)
        {
            //连接失败了，显示失败消息并关闭注册窗口
            std::cout << "connect to server failed" << std::endl;
            // loginDialog->emitRegister_succeeds_signal();
            return;
        }
        connectStatus = true;
        std::cout << "connect to server succeeds" << "sockfd = " << sockfd << std::endl;
    }
    
    //从注册窗口中获取用户名和密码
    QString userName = registerDialog->getRegisterUserName();
    QString passWord = registerDialog->getRegisterPassWord();

    //构建要发送的消息
    Register_Message msgToSend;
    Response_Message msgRespon;
    memset(&msgToSend, 0, sizeof(msgToSend));
    memset(&msgRespon, 0, sizeof(msgRespon));

    msgToSend.msgHead.opCode = E_REGISTER;
    msgToSend.msgHead.dataLen = sizeof(Register_Msg_Data);
    // strcpy(msgToSend.ipAddress, "127.0.0.1");//注意IP地址占用了16个字节，使用strcpy注意缓冲区溢出问题
    convertQstringtochar(msgToSend.regMsgData.name, &userName);
    convertQstringtochar(msgToSend.regMsgData.password, &passWord);
    //消息构造完成
    //发送消息到服务器,把注册信息保存到服务器上的Ｍysql数据库中
    sockCommu->writeToServer(sockfd, (void *)&msgToSend, sizeof(Register_Message));
    std::cout << "register_button_slot, write to server finished" << std::endl;
    //从服务器读取响应消息，读不到则阻塞
    sockCommu->readFromServer(sockfd, (void *)&msgRespon, sizeof(Response_Message));
    
    //在消息盒子里显示服务器对注册状态的响应
    QMessageBox msgBox;
    msgBox.setText(QString::fromLocal8Bit(msgRespon.respMsgData.response));
    msgBox.exec();
}


void ChatMainWindow::addFriend_action_slot()
{
    addFriendDialog = new AddFriendDialog(this);
    connect(addFriendDialog, SIGNAL(_add_friend_button_clicked_signal()),this, SLOT(addFriend_button_slot()));
    
    addFriendDialog->exec();  
}

void ChatMainWindow::addFriend_button_slot()
{
    //先与服务器建立连接
    if(connectStatus == false)//还未与服务器进行连接，需要连接
    {
        sockfd = sockCommu->socket_connect();
        if(sockfd == -1)
        {
            //连接失败了，显示失败消息并关闭注册窗口
            std::cout << "connect to server failed" << std::endl;
            // loginDialog->emitRegister_succeeds_signal();
            return;
        }
        connectStatus = true;
        std::cout << "connect to server succeeds" << "sockfd = " << sockfd << std::endl;
    }

    QString friendAccount = addFriendDialog->getFriendName();
    AddFriend_Message msgToSend;
    Response_Message msgRead;

    //构建要发送的消息
    memset(&msgToSend, 0, sizeof(msgToSend));


    msgToSend.msgHead.opCode = E_ADDFRIEND;
    msgToSend.msgHead.dataLen = sizeof(AddFriend_Msg_Data);
    // strcpy(msgToSend.ipAddress, "127.0.0.1");//注意IP地址占用了16个字节，使用strcpy注意缓冲区溢出问题
    convertQstringtochar(msgToSend.addFrienMsgData.currentUserName, &userName);
    convertQstringtochar(msgToSend.addFrienMsgData.friendAccount, &friendAccount);

    connect(this, SIGNAL(rcvAddFriendResponse_signal(bool, QString)),this, SLOT(addFriendResponse_slot(bool, QString)));

    //发送消息到服务器,把添加好友信息上传到服务器上的Ｍysql数据库中
    sockCommu->writeToServer(sockfd, (void *)&msgToSend, sizeof(AddFriend_Message));
    std::cout << "addFriend_button_slot, write to server finished" << std::endl;
    //从服务器读取响应消息，读不到则阻塞
    // sockCommu->readFromServer(sockfd, (void *)&msgRead, sizeof(Response_Message));
    
}

void ChatMainWindow::addFriendResponse_slot(bool operationStatus, QString responMsg)
{
    QMessageBox msgBox;
    //在消息盒子里显示服务器对添加好友请求的响应
    if( operationStatus == false)
    {
        msgBox.setText(responMsg);
        msgBox.exec();
    }
    else
    {
        addFriendDialog->close();
        //把好友信息添加到本地列表中，并为其创建相应的右键操作方式，还需要跟用户一起保存到数据库中（每个用户使用一个数据库）
        //从用户的数据库中读取好友列表，并依次返回
        addFriendToMyList(responMsg);
    }
}

void ChatMainWindow::addFriendToMyList(QString friendAccount)
{
    friend_WidgetItem = new QTreeWidgetItem(treeWidget, QStringList()<< friendAccount);

    //设置QTreeWidgetItem的各列成员的颜色，字体等
    QBrush brush_catalog = QBrush(QColor(199, 237, 225));//199, 237, 204
    friend_WidgetItem->setBackground(0, brush_catalog);

    friendList_WidgetItem_QList << friend_WidgetItem;

    treeWidget->insertTopLevelItems(0, friendList_WidgetItem_QList);
}

/*void ChatMainWindow::find()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &,
                                            Qt::CaseSensitivity)),
                spreadsheet, SLOT(findNext(const QString &,
                                           Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &,
                                                Qt::CaseSensitivity)),
                spreadsheet, SLOT(findPrevious(const QString &,
                                               Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}*/

void ChatMainWindow::about()
{
    QMessageBox::about(this, tr("About Chat App"),
            tr("<h2>Chat App 0.1</h2>"
               "<p>Copyright &copy; 2020 Software Inc."
               "<p>Chat App is a small application that "
               "I can test and study Qt and Network Programming."));
}

/*void ChatMainWindow::enableSearchButton(const QString &text)
{
    searchButton->setEnabled(!text.isEmpty());
}*/

/*void ChatMainWindow::updateStatusBar()
{
    
}*/

void ChatMainWindow::createActions()
{
    register_Action = new QAction(tr("Register"), this);
    register_Action->setIcon(QIcon(":/images/register.jpg"));
    connect(register_Action, SIGNAL(triggered()), this, SLOT(register_action_slot()));

    logIn_Action = new QAction(tr("Log in"), this);
    logIn_Action->setIcon(QIcon(":/images/login.jpg"));
    connect(logIn_Action, SIGNAL(triggered()), this, SLOT(logIn_action_slot()));


    addFriend_Action = new QAction(tr("Add Friend"), this);
    addFriend_Action->setIcon(QIcon(":/images/add_friend.jpg"));
    addFriend_Action->setStatusTip(tr(" "));
    connect(addFriend_Action, SIGNAL(triggered()), this, SLOT(addFriend_action_slot()));

    quit_Action = new QAction(tr("Q&uit"), this);
    quit_Action->setIcon(QIcon(":/images/close.png"));
    quit_Action->setShortcut(tr("Ctrl+Q"));
    quit_Action->setStatusTip(tr("Exit the application"));
    connect(quit_Action, SIGNAL(triggered()), this, SLOT(close()));

    // update_Action = new QAction(tr("&Update..."), this);
    // update_Action->setIcon(QIcon(":/images/refresh.png"));
    // update_Action->setShortcut(QKeySequence::Find);
    // connect(update_Action, SIGNAL(triggered()), this, SLOT(goThroughDatabase()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));//qApp是一个全局指针，指向QApplication对象
}

void ChatMainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Operation"));
    fileMenu->addAction(register_Action);
    fileMenu->addAction(logIn_Action);
    // fileMenu->addAction(addFriend_Action);
    // fileMenu->addAction(update_Action);
    fileMenu->addAction(quit_Action);
    // separatorAction = fileMenu->addSeparator();
    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

/*void ChatMainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}*/

void ChatMainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&Add a friend"));
    fileToolBar->addAction(register_Action);
    fileToolBar->addAction(logIn_Action);
    fileToolBar->addAction(addFriend_Action);
    //fileToolBar->addAction(update_Action);
    fileToolBar->addAction(quit_Action);
}

/*void ChatMainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()),
            this, SLOT(spreadsheetModified()));

    updateStatusBar();
}*/


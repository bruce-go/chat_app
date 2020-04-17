#include <QApplication>//头文件中声明了class QDesktopWidget;但是并没有包含这个类相关的头文件，
// 所以必须在下面包含这个头文件，app->desktop()才能正确调用QDesktopWidget类的成员
#include <iostream>

#include <QDesktopWidget>//如果不包含这个头文件，会报错error: invalid use of incomplete type ‘class QDesktopWidget’

#include "chatMainWindow.h"

#include "workThread.h"
// #include "cd_dbm.h"
#include "loginDialog.h"

// MainWindow *mainWin;
QApplication *app;

int main(int argc, char *argv[])
{
    int rtnVal;
    // LoginDialog *loginDia;
    ChatMainWindow *mainWindow;
    WorkThread *workThread;
    //QApplication app(argc, argv);
    app = new QApplication(argc, argv);

    mainWindow = new ChatMainWindow();

    workThread = new WorkThread(mainWindow);

    workThread->start();
    //创建线程
    // mainWindow->startThread();

    // mainWindow->loginDialog = new LoginDialog();

    const QRect screen = app->desktop()->screenGeometry(mainWindow);
     // mainWindow->loginDialog->move(screen.center() - mainWindow->loginDialog->rect().center());
    /*给mainWindow设置一个合适的初始大小，并让其位于桌面的中央*/
    // loginDia->resize(500, 500);
    
    // loginDia->show();
    /*if(mainWindow->getLoginStatus())//如果登录状态成功，则显示主窗口，否则不显示
    {*/
    // mainWindow->setModal(false);
    mainWindow->show();
    /*}*/
    
    //登录窗口登录成功以后，关闭登录窗口，然后显示一个主窗口，其中包含自己的个人信息和好友列表
    return app->exec();
}

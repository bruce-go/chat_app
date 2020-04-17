#include <QtGui>

#include "loginDialog.h"

class MainWindow;

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
	label_LogInUserName = new QLabel(tr("User Name"));
    lineEdit_LogInUserName = new QLineEdit;
    label_LogInUserName->setBuddy(lineEdit_LogInUserName);

    label_LogInPassWord = new QLabel(tr("Password"));
    lineEdit_LogInpassWord = new QLineEdit;
    label_LogInPassWord->setBuddy(lineEdit_LogInpassWord);

    loginButton = new QPushButton(tr("&Log in"));
    loginButton->setDefault(true);
    loginButton->setEnabled(false);

    // registerButton = new QPushButton(tr("Register"));

    connect(lineEdit_LogInUserName, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableLogInButton_slot(const QString &)));

    connect(loginButton, SIGNAL(clicked()),this, SLOT(logInButtonClicked_slot()));

    // connect(registerButton, SIGNAL(clicked()),this, SLOT(registerButtonClicked_slot()));

    QHBoxLayout *herizontalLayout_1 = new QHBoxLayout;
    herizontalLayout_1->addWidget(label_LogInUserName);
    herizontalLayout_1->addWidget(lineEdit_LogInUserName);

    QHBoxLayout *herizontalLayout_2 = new QHBoxLayout;
    herizontalLayout_2->addWidget(label_LogInPassWord);
    herizontalLayout_2->addWidget(lineEdit_LogInpassWord);

    QHBoxLayout *herizontalLayout_3 = new QHBoxLayout;
    herizontalLayout_3->addStretch();
    herizontalLayout_3->addWidget(loginButton);
    // herizontalLayout_3->addWidget(registerButton);
    herizontalLayout_3->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    // mainLayout->addStretch();
    mainLayout->addLayout(herizontalLayout_1);
    mainLayout->addLayout(herizontalLayout_2);
    mainLayout->addLayout(herizontalLayout_3);
    mainLayout->addStretch();
    setLayout(mainLayout);

    setWindowTitle(tr("User Login"));
    setFixedHeight(sizeHint().height());

}

QString LoginDialog::getLogInUserName()
{
    return text_LogInUserName;
}

QString LoginDialog::getLogInPassWord()
{
    return text_LogInPassWord;
}

/*QString LoginDialog::getRegisterUserName()
{
    return text_RegisterUserName;
}

QString LoginDialog::getRegisterPassWord()
{
    return text_RegisterPassWord;
}*/

void LoginDialog::logInButtonClicked_slot()
{
    //先保存对话框的输入内容
    //还可以添加对输入信息长度的检测，如果超过指定长度则在报错（弹出对话框或者在状态栏用红色字体显示错误原因）
    text_LogInUserName = lineEdit_LogInUserName->text();
    text_LogInPassWord = lineEdit_LogInpassWord->text();

    //然后清除掉，以免多次按下add会导致重复添加相同的条目
    lineEdit_LogInUserName->clear();
    lineEdit_LogInpassWord->clear();
    
    //在这里再次发送信号，自定义一个
    emit _login_button_clicked_signal(); 
}

/*void LoginDialog::registerButtonClicked_slot()
{
    //先弹出对话框，填写用户名和密码，并保存下来
    registerDialog = new RegisterDialog();
    connect(this, SIGNAL(_register_succeeds_signal()),registerDialog, SLOT(close()));
    // registerDialog->show();
    // registerDialog->raise();
    // registerDialog->activateWindow();
    registerDialog->exec();

    text_RegisterUserName = registerDialog->getRegisterUserName();
    text_RegisterPassWord = registerDialog->getRegisterPassWord();
     //只有点击了register对话框的ok按钮，才发送信号
    if(registerDialog->getRegisterExitStatus())
    {
        emit _register_button_clicked_signal();
    }
    
}*/


void LoginDialog::enableLogInButton_slot(const QString &text)
{
    loginButton->setEnabled(!text.isEmpty());
}

/*void LoginDialog::emitRegister_succeeds_signal()
{
    emit _register_succeeds_signal();
}*/

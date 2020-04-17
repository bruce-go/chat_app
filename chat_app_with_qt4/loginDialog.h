#ifndef ADDCDDIALOG_H
#define ADDCDDIALOG_H

#include <QDialog>
// #include "registerDialog.h"

class QLabel;
class QLineEdit;
class QPushButton;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = 0);
    // LoginDialog(QWidget *parent, QString catalog_entry, QString catalog_title, QString catalog_artist, QString catalog_type);
    /*提供公有函数来返回获取到的CD目录信息*/
    QString getLogInUserName();
    QString getLogInPassWord();
    // QString getRegisterUserName();
    // QString getRegisterPassWord();
    // void emitRegister_succeeds_signal();

signals:
    void _login_button_clicked_signal();
    // void _register_button_clicked_signal();
    

private slots:
    void logInButtonClicked_slot();
    // void registerButtonClicked_slot();
    void enableLogInButton_slot(const QString &text);
  
private:
    QLabel *label_LogInUserName;
    QLabel *label_LogInPassWord;

    QLineEdit *lineEdit_LogInUserName;
    QLineEdit *lineEdit_LogInpassWord;
    
    QPushButton *loginButton;
    // QPushButton *registerButton;
    
    QString text_LogInUserName;
    QString text_LogInPassWord;

    // QString text_RegisterUserName;
    // QString text_RegisterPassWord;

    // RegisterDialog *registerDialog;
};

#endif

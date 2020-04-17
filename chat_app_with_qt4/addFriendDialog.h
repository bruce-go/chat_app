#ifndef ADDFRIENDCDDIALOG_H
#define ADDFRIENDCDDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    AddFriendDialog(QWidget *parent = 0);
    // LoginDialog(QWidget *parent, QString catalog_entry, QString catalog_title, QString catalog_artist, QString catalog_type);
    /*提供公有函数来返回获取到的CD目录信息*/
    QString getFriendName();
    // QString getLogInPassWord();
    // QString getRegisterUserName();
    // QString getRegisterPassWord();
    // void emitRegister_succeeds_signal();

signals:
    void _add_friend_button_clicked_signal();
    // void _register_button_clicked_signal();
    

private slots:
    void addFriendButtonClicked_slot();
    // void registerButtonClicked_slot();
    void enableAddFriendButton_slot(const QString &text);
  
private:
    QLabel *label_FriendName;

    QLineEdit *lineEdit_FriendName;
    // QLineEdit *lineEdit_FriendpassWord;
    
    QPushButton *addFriendButton;
    QPushButton *cancelButton;
    
    QString text_FriendName;
   

    
};

#endif

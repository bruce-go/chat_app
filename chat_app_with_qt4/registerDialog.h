#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    RegisterDialog();

    QString getRegisterUserName();
    QString getRegisterPassWord();
    bool    getRegisterExitStatus();

signals:
    void _register_confirmed_signal();

private slots:
    void enableOkButton_slot(const QString &text);
    void okButtonClicked_slot();

private:
    QLabel *label_RegisterUserName;
    QLabel *label_RegisterPassWord;

    QLineEdit *lineEdit_RegisterUserName;
    QLineEdit *lineEdit_RegisterpassWord;
    
    QPushButton *okButton;
    QPushButton *cancelButton;
    
    QString text_RegisterUserName;
    QString text_RegisterPassWord;

    bool text_registerExitStatus = false;

};

#endif

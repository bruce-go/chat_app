#include <QtGui>

#include "registerDialog.h"

RegisterDialog::RegisterDialog()
{
    label_RegisterUserName = new QLabel(tr("New User Name"));
    lineEdit_RegisterUserName = new QLineEdit;
    label_RegisterUserName->setBuddy(lineEdit_RegisterUserName);

    label_RegisterPassWord = new QLabel(tr("Password"));
    lineEdit_RegisterpassWord = new QLineEdit;
    label_RegisterPassWord->setBuddy(lineEdit_RegisterpassWord);

    okButton = new QPushButton(tr("Ok"));
    okButton->setDefault(true);
    okButton->setEnabled(false);

    cancelButton = new QPushButton(tr("Cancel"));

    connect(lineEdit_RegisterUserName, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton_slot(const QString &)));

    connect(okButton, SIGNAL(clicked()),this, SLOT(okButtonClicked_slot()));

    connect(cancelButton, SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *herizontalLayout_1 = new QHBoxLayout;
    herizontalLayout_1->addWidget(label_RegisterUserName);
    herizontalLayout_1->addWidget(lineEdit_RegisterUserName);

    QHBoxLayout *herizontalLayout_2 = new QHBoxLayout;
    herizontalLayout_2->addWidget(label_RegisterPassWord);
    herizontalLayout_2->addWidget(lineEdit_RegisterpassWord);

    QHBoxLayout *herizontalLayout_3 = new QHBoxLayout;
    herizontalLayout_3->addStretch();
    herizontalLayout_3->addWidget(okButton);
    herizontalLayout_3->addWidget(cancelButton);
    herizontalLayout_3->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    // mainLayout->addStretch();
    mainLayout->addLayout(herizontalLayout_1);
    mainLayout->addLayout(herizontalLayout_2);
    mainLayout->addLayout(herizontalLayout_3);
    mainLayout->addStretch();
    setLayout(mainLayout);

    setWindowTitle(tr("User Register"));
    setFixedHeight(sizeHint().height());
}

void RegisterDialog::enableOkButton_slot(const QString &text)
{
    okButton->setEnabled(!text.isEmpty());
}

void RegisterDialog::okButtonClicked_slot()
{
	text_RegisterUserName = lineEdit_RegisterUserName->text();
	text_RegisterPassWord = lineEdit_RegisterpassWord->text();

	text_registerExitStatus = true;

    lineEdit_RegisterUserName->clear();
    lineEdit_RegisterpassWord->clear();

    if(text_registerExitStatus == true)
    {
        emit _register_confirmed_signal();
    }
}

QString RegisterDialog::getRegisterUserName()
{
	return text_RegisterUserName;
}

QString RegisterDialog::getRegisterPassWord()
{
	return text_RegisterPassWord;
}

bool RegisterDialog::getRegisterExitStatus()
{
	return text_registerExitStatus;
}
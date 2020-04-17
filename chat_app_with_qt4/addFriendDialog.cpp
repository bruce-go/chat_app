#include <QtGui>
#include "addFriendDialog.h"

AddFriendDialog::AddFriendDialog(QWidget *parent) : QDialog(parent)
{
	label_FriendName = new QLabel(tr("Friend Name"));
    lineEdit_FriendName = new QLineEdit;
    label_FriendName->setBuddy(lineEdit_FriendName);

    addFriendButton = new QPushButton(tr("&Add"));
    addFriendButton->setDefault(true);
    addFriendButton->setEnabled(false);

    cancelButton = new QPushButton(tr("Cancel"));

    connect(lineEdit_FriendName, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableAddFriendButton_slot(const QString &)));

    connect(addFriendButton, SIGNAL(clicked()),this, SLOT(addFriendButtonClicked_slot()));

    connect(cancelButton, SIGNAL(clicked()),this, SLOT(close()));

    QHBoxLayout *herizontalLayout_1 = new QHBoxLayout;
    herizontalLayout_1->addWidget(label_FriendName);
    herizontalLayout_1->addWidget(lineEdit_FriendName);

    QHBoxLayout *herizontalLayout_3 = new QHBoxLayout;
    herizontalLayout_3->addStretch();
    herizontalLayout_3->addWidget(addFriendButton);
    herizontalLayout_3->addWidget(cancelButton);
    herizontalLayout_3->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(herizontalLayout_1);
    mainLayout->addLayout(herizontalLayout_3);
    mainLayout->addStretch();
    setLayout(mainLayout);

    setWindowTitle(tr("Add Friend"));
    setFixedHeight(sizeHint().height());
}



void AddFriendDialog::enableAddFriendButton_slot(const QString &text)
{
	addFriendButton->setEnabled(!text.isEmpty());
}

void AddFriendDialog::addFriendButtonClicked_slot()
{
	text_FriendName = lineEdit_FriendName->text();

	lineEdit_FriendName->clear();

	emit _add_friend_button_clicked_signal();
}

QString AddFriendDialog::getFriendName()
{
	return text_FriendName;
}
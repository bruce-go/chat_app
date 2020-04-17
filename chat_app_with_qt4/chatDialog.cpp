/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "chatDialog.h"

ChatDialog::ChatDialog(const QString &curUserName,QString friendName, QWidget *parent)
    : QDialog(parent)
{
    //setupUi(this);
    label_MsgToSend = new QLabel(tr("Message"));
    textEdit = new QTextEdit();
    lineEdit_Msg = new QLineEdit();
    listWidget = new QListWidget();
    tableFormat = new QTextTableFormat();

    lineEdit_Msg->setFocusPolicy(Qt::StrongFocus);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    listWidget->setFocusPolicy(Qt::NoFocus);

    connect(lineEdit_Msg, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

    myName = curUserName;//client.nickName();
    friendChatWith = friendName;
    newParticipant(friendName);
    tableFormat->setBorder(0);
    // QTimer::singleShot(10 * 1000, this, SLOT(showInformation()));

    QHBoxLayout *herizontalLayout_up = new QHBoxLayout;
    herizontalLayout_up->addWidget(textEdit);
    herizontalLayout_up->addWidget(listWidget);

    QHBoxLayout *herizontalLayout_down = new QHBoxLayout;
    herizontalLayout_down->addWidget(label_MsgToSend);
    herizontalLayout_down->addWidget(lineEdit_Msg);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(herizontalLayout_up);
    mainLayout->addLayout(herizontalLayout_down);
   
    // mainLayout->addStretch();
    setLayout(mainLayout);

    QString title = QString("Chatting with ") + friendName + QString("...");

    setWindowTitle(title);
}

//可以根据用户名来区分设计，自己和好友所发送的消息的颜色和字号
void ChatDialog::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QString message_decorated;
    QTextCursor cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    /*QTextTable *table = cursor.insertTable(1, 2, *tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from +" said: "+ "> ");
    if(from == myName)
    {
        message_decorated = QString("<font color=\'blue\'>%1").arg(message);
    }
    else
    {
        message_decorated = QString("<font color=\'yellow\'>%1").arg(message);
    }

    table->cellAt(0, 1).firstCursorPosition().insertText(message_decorated);*/
    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::blue);
    textEdit->append(tr("< %1 said: >").arg(from));
    textEdit->setTextColor(color);

    color = textEdit->textColor();
    if(from == myName)
    {
        
        textEdit->setTextColor(Qt::red);
        
    }
    else
    {
        textEdit->setTextColor(Qt::black);
    }
    textEdit->append(tr("%1").arg(message));
    textEdit->setTextColor(color);

    QScrollBar *bar = textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDialog::returnPressed()
{
    text = lineEdit_Msg->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = textEdit->textColor();
        textEdit->setTextColor(Qt::red);
        textEdit->append(tr("! Unknown command: %1")
                         .arg(text.left(text.indexOf(' '))));
        textEdit->setTextColor(color);
    } else {
        // client.sendMessage(text);
        appendMessage(myName, text);
        emit sendMessageToFriend_signal(currentDiologIdx, friendChatWith);
    }

    lineEdit_Msg->clear();
}

void ChatDialog::newParticipant(const QString &friendName)
{
    if (friendName.isEmpty())
        return;

    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::blue);
    textEdit->append(tr("****** Talk to %1 ******").arg(friendName));
    textEdit->setTextColor(color);
    listWidget->addItem(friendName+tr(" is in chat room"));
}

void ChatDialog::participantLeft(const QString &friendName)
{
    if (friendName.isEmpty())
        return;

    QList<QListWidgetItem *> items = listWidget->findItems(friendName,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* %1 has left").arg(friendName));
    textEdit->setTextColor(color);
}

void ChatDialog::showInformation()
{
    if (listWidget->count() == 1) {
        QMessageBox::information(this, tr("Chat"),
                                 tr("Launch several instances of this "
                                    "program on your local network and "
                                    "start chatting!"));
    }
}


QString ChatDialog::getMessage()
{
    return text;
}

void ChatDialog::setCurrentDialogInx(unsigned int idx)
{
    currentDiologIdx = idx;
}

void ChatDialog::saveFriendChattingWith(QString friendAccount)
{
    friendChatWith = friendAccount;
}
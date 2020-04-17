#ifndef WORK_THREAD_H
#define WORK_THREAD_H

#include <QThread>

// class QThread;

#include "chatMainWindow.h"

class WorkThread : public QThread
{
public:
	WorkThread(ChatMainWindow *chatMainWin)
	{
		chatMainWinPtr = chatMainWin;
	}

private:
	virtual void run()
	{
		
		chatMainWinPtr->rcvAndDispatchChatMsg_thread();
	}

	ChatMainWindow *chatMainWinPtr;

};


#endif
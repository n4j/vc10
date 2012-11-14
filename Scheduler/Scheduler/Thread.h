
#include "stdafx.h"
#include <windows.h>

class Thread {

private:
	DWORD m_threadid;
	HANDLE m_thread;
	void* m_args;
	LPTHREAD_START_ROUTINE m_thread_func;
	
public:
	
	explicit Thread(LPTHREAD_START_ROUTINE thread, void* args = NULL);
	explicit Thread(HANDLE thread);
	
	static Thread* Current();

	operator LPTHREAD_START_ROUTINE();
	void operator = (Thread* other);

	void Execute();
	void Wait(DWORD millis = INFINITE);
	void Terminate();
	void Cleanup();

private:
	Thread(){}
	Thread(const Thread& other){}
};


	Thread::Thread(LPTHREAD_START_ROUTINE thread, void* args):
		m_thread_func(thread),
		m_threadid(-1),
		m_thread(INVALID_HANDLE_VALUE),
		m_args(args){}

	Thread::Thread(HANDLE thread):
		m_thread_func(NULL),
		m_threadid(GetCurrentThreadId()),
		m_thread(thread) {  }

	void Thread::Execute() {
		if(m_thread_func==NULL) return;
		m_thread = CreateThread(NULL,0,m_thread_func,m_args,0,&m_threadid);
	}

	void Thread::Wait(DWORD millis){
		DWORD return_val = WaitForSingleObject(m_thread,millis);
		if(return_val == 0xFFFFFF){
			DWORD error_code = GetLastError();
			printf("%x",error_code);
		}
	}

	void Thread::Terminate() {
		TerminateThread(m_thread,-1);
		Cleanup();
	}

	void Thread::Cleanup() {
		this->m_thread = INVALID_HANDLE_VALUE;
		this->m_threadid = -1;
		this->m_thread_func= NULL;
	}

	Thread* Thread::Current() {
		Thread* current = new Thread( GetCurrentThread() );
		return current;
	}

	
	Thread::operator LPTHREAD_START_ROUTINE() {
		return m_thread_func;
	}

	void Thread::operator=(Thread* other) {
		if(NULL == other) return;
		m_thread = other->m_thread;
		m_thread_func = other->m_thread_func;
		m_threadid = other->m_threadid;
	}
// Scheduler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Scheduler.h"

template<class T> class SampleTask: public Task<T> {

private:
	bool m_completed;

public:

	SampleTask():
		m_completed(false){
	}

	virtual void Run() {
		for(int i=0; i<10; i++) {
			DWORD thread_id = GetCurrentThreadId();
			printf("%d: Marry had a little lamb...\n",thread_id);
		}
	}

	virtual void TaskCanceled() {
		m_completed = true;
	}

	virtual void MarkCompleted() {
		assert( (true != m_completed) );
		m_completed = true;
	}

	virtual bool IsCompleted() {
		return m_completed;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	ThreadPoolExecutor* executor = new ThreadPoolExecutor(8);
	
	for(int i=0; i<10000; i++) {
		SampleTask<int>* task = new SampleTask<int>();
		executor->Submit((Task<class T>*)task);
	}

	Sleep(200000);
	return 0;
}


#include <queue>

#include"stdafx.h"
#include "ScopedLock.h"
#include "Thread.h"

using namespace std;

#pragma once

class Future;
 DWORD __stdcall ThreadFunc( void* args );

/* Represents a |Task| submitted to an |Executor|.
 * The task may or may not be cancellable.
 */
template<class T> class Task {

	friend class Future;

public:
	virtual void Run() = 0;
	virtual void TaskCanceled() = 0;
	virtual void MarkCompleted() = 0;

private:
	virtual bool IsCompleted() = 0;
};
//^_^


/* Encapuslates a |Task| which would be completed in |Future|.*/
class Future {

private:
	Task<class T>* m_task;
	
public:
	Future(Task<class T>* task);

public:
	inline bool IsCompleted();
	inline void Cancel();

private:
	Future(Future& other);
	Future();
};
//^-^

Future::Future(Task<class T>* task):
	m_task(task){
}

bool Future::IsCompleted() {
	return m_task->IsCompleted();
}

void Future::Cancel() {
	m_task->TaskCanceled();
}

class Executor {

public:

	virtual Future* Submit(Task<class T>* task) = 0;
	virtual Task<class T>* GetNextTask() = 0;
};

class ThreadPoolExecutor: public Executor {

private:
	size_t m_pool_size;
	HANDLE m_event;
	queue<Task<class T>* >* m_task_queue;
	Thread** m_thread_group;
	CRITICAL_SECTION m_fetch_mutex;

public:
	ThreadPoolExecutor( size_t m_pool_size );

	virtual Future* Submit(Task<class T>* task);
	virtual Task<class T>* GetNextTask();

private:
	void InitalizeWorkerThreads();
}; 

ThreadPoolExecutor::ThreadPoolExecutor( size_t m_pool_size ):
	m_task_queue(0),
	m_pool_size(m_pool_size),
	m_event(INVALID_HANDLE_VALUE){

		m_thread_group = new Thread*[m_pool_size];
		m_task_queue = new queue<Task<class T>* >();
		m_event = CreateEventEx(NULL,L"ExecutorQueueEvent",0,EVENT_ALL_ACCESS);

		InitializeCriticalSection(&m_fetch_mutex);
		ResetEvent(m_event);

		InitalizeWorkerThreads();
}

Future* ThreadPoolExecutor::Submit(Task<class T>* task){

	ScopedLock lock(m_fetch_mutex);

	Future* future = new Future(task);

	assert( (NULL != task) );
	assert( (NULL != m_task_queue) );
	m_task_queue->push(task);
	SetEvent(m_event);
	return future;
}

void ThreadPoolExecutor::InitalizeWorkerThreads() {

	for(size_t i=0; i<m_pool_size; i++) {
		m_thread_group[i] = new Thread(ThreadFunc,this);
	}

	for(size_t i=0; i<m_pool_size; i++) {
		m_thread_group[i]->Execute();
	}
}

Task<class T>* ThreadPoolExecutor::GetNextTask() {

	Task<class T>* next_task = 0;
	
	//Multiple threads will be blocked at this event
	WaitForSingleObject(m_event,INFINITE);
	
	//Only one of them would pass through the mutex
	ScopedLock mutex(m_fetch_mutex);

	assert( ( NULL != m_task_queue ) );
	assert( (  m_task_queue->size() > 0 ) );
	next_task = m_task_queue->front();
	m_task_queue->pop();

	//If there are tasks left, wake up one thread
	if(0 == m_task_queue->size())ResetEvent(m_event);
	else SetEvent(m_event);

	return next_task;
}

 DWORD __stdcall ThreadFunc( void* args ) {

	 
	 Executor* executor = reinterpret_cast<Executor*>(args);
	 assert( (NULL != executor) );

	 Task<class T>* task = 0;

	 while( NULL != (task = executor->GetNextTask()) ) {
		 __try {
			 task->Run();
			 task->MarkCompleted();
		 }
		 __except(0){
		 }
	 }

	 return 0;
}

#include "stdafx.h"
#pragma once

class ScopedLock
{
private:
	CRITICAL_SECTION m_lock;

public:
	explicit ScopedLock(CRITICAL_SECTION lock);
	~ScopedLock();

// ^_^ DISALLOW EVIL CONSTRUCTORS 
private:
	ScopedLock(){};
	ScopedLock(ScopedLock& other){};
};

ScopedLock::ScopedLock(CRITICAL_SECTION lock)
	:m_lock(lock){
	EnterCriticalSection(&m_lock);
}

ScopedLock::~ScopedLock() {
	LeaveCriticalSection(&m_lock);
}
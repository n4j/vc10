
#include <crtdbg.h>
#include "stdafx.h"
#include "Thread.h"
#include "ScopedLock.h"

CRITICAL_SECTION mutex;


DWORD __stdcall SomeWork(void*){
	
	
	DWORD thread_id = GetCurrentThreadId();
	
	LockScope(
		
		ScopedLock lock(mutex) ,
		
		for(int i=0; i<10; i++){
			printf("%d: Quick Brown Fox Jumps Over The Lazy Dog.\n",thread_id);
		}
	);
	
	
	return -1;
}

#define SIZEOF_PTR 4

static inline size_t
pow2_ceil(size_t x)
{

	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
#if (SIZEOF_PTR == 8)
	x |= x >> 32;
#endif
	x++;
	return (x);
}

int _tmain(int argc, _TCHAR* argv[]){

	size_t po = pow2_ceil(99);

	//InitializeCriticalSection(&mutex);

	//int num_threads = 4*2000;

	//Thread** t = (Thread**)malloc(sizeof(Thread)*num_threads);

	//for(int i=0; i<num_threads; i++) {
	//	t[i] = new Thread(SomeWork);
	//	t[i]->Execute();
	//}

	//for(int i=0; i<num_threads; i++) {
	//	t[i]->Wait();
	//}

	//for(int i=0; i<num_threads; i++) {
	//	t[i]->Cleanup();
	//	delete t[i];
	//}

	//free(t);
	//t=0;
	//
	//DeleteCriticalSection(&mutex);

	//_CrtDumpMemoryLeaks();


	
	return 0;
}
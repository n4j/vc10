
#include <string.h>

#include "stdafx.h"
#include "LinkList.h"
#include "RefPtr.h"

void inline assert(bool condition){
	if(!condition)
		__asm int 3;
}

 template <class T> inline T* Malloc(size_t size) {

	 return dynamic_cast<T>(malloc(size));
	//return (T*)malloc(size);
}

int main(){
	int* d = Malloc<int>(200);
	RefPtr<LinkList<int>> int_list = new LinkList<int>();

	for(int i=0; i<250000;i++){
		int_list->Add(i);		
	}

	for(int i=0; i<250000;i++){
		assert( int_list->Get(i) == i );
	}
	
	return 0;
}
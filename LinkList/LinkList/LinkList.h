#include "stdafx.h"

#pragma once

namespace kronos{
	namespace Commons {

template<class T> class LinkList{

private:
	typedef struct internal_LinkList{
		T value;
		internal_LinkList* next;
	};


	internal_LinkList* link_list;
	internal_LinkList* next_free;
	
	void** index;
	int current_index;
	int capacity;

	int m_ref;

public:
	
	LinkList(void):
		link_list((internal_LinkList*)malloc(sizeof( internal_LinkList ))),
		next_free(link_list),
		capacity(100),
		index(new void*[100*sizeof(void*)]),
		current_index(0),
		m_ref(0){

			link_list->next=0;
			memset(index,0,100*sizeof(void*));			
	}

	void Add(T value) {

		  next_free->value = value;

		  AddToIndex(next_free);

		  next_free->next = (internal_LinkList*)malloc(sizeof( internal_LinkList ));
		  next_free = next_free->next;
		  next_free->next = 0;
	 }

private:

	void AddToIndex(internal_LinkList* entry ){

		if(current_index == capacity-1){
			
			//void* start_from = 0;
			
			capacity+=100;
			index = (void**)realloc(index,capacity*sizeof(void*));
			//start_from = index+ ( (capacity-100) * sizeof(void*) );
			//memset( start_from,0,100 );
		}

		index[current_index++] = (void*)entry;
	}

	void Destroy() {

		internal_LinkList* current = link_list;
		internal_LinkList* previous = 0;

		while(current){
			previous = current;
			current = previous->next;
			free(previous);
			previous = 0;
		}

		if(index){
			free(index);
			index = 0;
		}

		link_list=0;
		next_free=0;
	}

public:

	T Get(int at){

		if( at < 0 || at >= current_index ){
			return (T)(0);
		}

		internal_LinkList* valueAt = (internal_LinkList*)index[at];

		if(!valueAt) {
			return (T)(0);
		}

		return valueAt->value;
	}

	int ref() {
		return m_ref++;
	}

	int deref(){
		int previous_count = m_ref--;
		
		if(!m_ref){
			Destroy();
		}

		return previous_count;
	}

~LinkList(void){

	Destroy();
}


};
}}

using kronos::Commons::LinkList;
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
using namespace std;


typedef void(*callback_handler)(void * data);

typedef struct event{
	callback_handler handler;
	void * data;
} Event;

typedef struct node{
	struct node * next;
	Event e;
}Node;


bool trigger = false;
Node * queue_head = NULL;
Node * cur_ptr = queue_head; 

void init(){
	trigger = false;
}

void openTrigger(){
	trigger = true;
}

void closeTrigger(){
	trigger = false;
}


bool callback_register(Event * e){
	if(trigger == false){
		Node * node = (Node *) malloc(sizeof(Node));
		node->next =  NULL;
		if(cur_ptr == NULL){
			queue_head = node;
		}
		else{
			cur_ptr->next = node;
		}
		//here use memcpy, cause the client may want to free the event after it call the register
		memcpy(&(node->e),e,sizeof(Event));
		cur_ptr = node;
		printf("callback_register:successfully cur_ptr:%p\n",(char *)cur_ptr);
	}
	else{
		//this should begin after all callbacks are invoked in queue?
		e->handler(e->data);	

		//TODO:free this event or not
	}
	return true;
}

void event_fire(){
	if(trigger == true && queue_head != NULL){
		do{
			printf("begin event fire %p\n",queue_head);
			Node * node = queue_head;
			Event * e = &(node->e);
			queue_head = queue_head->next;
			e->handler(e->data);
			free(node);
		}while(trigger == true && queue_head != NULL);
	}
	
}

//Test callback functions
void func1(void * data1){
	printf("callback function 1:%p\n",data1);
}
void func2(void * data2){
	printf("callback function 2:%p\n",data2);
}
void func3(void * data3){
	printf("callback function 3:%p\n",data3);
}
void func4(void * data4){
	printf("callback function 4:%p\n",data4);
}
void func5(void * data5){
	printf("callback function 5:%p\n",data5);
}

int main(int argc, char ** argv){
	init();
	Event * e1 =(Event *) malloc(sizeof(Event));
	e1->handler = func1;
	e1->data = (void *)1;
	Event * e2 =(Event *) malloc(sizeof(Event));
	e2->handler = func2;
	e2->data = (void *)2;
	Event * e3 =(Event *) malloc(sizeof(Event));
	e3->handler = func3;
	e3->data = (void *)3;
	Event * e4 =(Event *) malloc(sizeof(Event));
	e4->handler = func4;
	e4->data = (void *)4;
	Event * e5 =(Event *) malloc(sizeof(Event));
	e5->handler = func5;
	e5->data = (void *)5;

	//register callback: push into queue
	callback_register(e1);
	callback_register(e2);
	callback_register(e3);
	callback_register(e4);
	callback_register(e5);

	openTrigger();

	event_fire();
	callback_register(e1);


}
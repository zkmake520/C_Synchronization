#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <mutex> 
#include <thread>
#include <condition_variable>
using namespace std;


typedef void(*callback_handler)(void * data);

typedef struct event{
	callback_handler handler;
	void * data;
} Event;


bool trigger = false;
mutex mtx;
mutex outMtx;
condition_variable cv;
queue<Event *> event_queue; 

bool callback_register(Event * e){
	int size ;

	Event * node = (Event *) malloc(sizeof(Event));
	memcpy(node,e,sizeof(Event));
	mtx.lock();
	event_queue.push(node);
	size = event_queue.size();
	mtx.unlock();
	outMtx.lock();
	cout<<"register successfully size "<<size<<endl;
	outMtx.unlock();
	cv.notify_all();
	return true;
}

void invokeTrigger(){
	trigger =true;
	cv.notify_all();
}
void closeTrigger(){
	trigger = false;
}
bool callback_available(){
	return trigger == true && !event_queue.empty(); 
}
void event_fire(){
	while(true){
		unique_lock<mutex> lck(mtx);
		cv.wait(lck,callback_available);
		outMtx.lock();
		cout<<this_thread::get_id()<<" is wake up "<<endl;
		outMtx.unlock();
		lck.unlock();
		while(trigger == true){
			Event * e;
			mtx.lock();
			if(!event_queue.empty()){
				e = event_queue.front();
				event_queue.pop();
				mtx.unlock();
				e->handler(e->data);
				free(e);
			}
			else{
				mtx.unlock();
				break;
			}
		
		}
	}	
}

//Test callback functions
void func1(void * data1){
	outMtx.lock();
	cout<<"callback function 1:"<<data1<<"  id"<<this_thread::get_id()<<endl;
	outMtx.unlock();
}
void func2(void * data2){
	outMtx.lock();
	cout<<"callback function 2:"<<data2<<"  id"<<this_thread::get_id()<<endl;
	outMtx.unlock();
}
void func3(void * data3){
	outMtx.lock();
	cout<<"callback function 3:"<<data3<<"  id"<<this_thread::get_id()<<endl;
	outMtx.unlock();
}
void func4(void * data4){
	outMtx.lock();
	cout<<"callback function 4:"<<data4<<"  id"<<this_thread::get_id()<<endl;
	outMtx.unlock();
}
void func5(void * data5){
	outMtx.lock();
	cout<<"callback function 5:"<<data5<<"  id"<<this_thread::get_id()<<endl;
	outMtx.unlock();
}

int main(int argc, char ** argv){
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

	thread t1(callback_register,e1);
	thread t2(callback_register,e2);
	thread w1(event_fire);
	thread w2(event_fire);
	thread t3(callback_register,e3);
	thread t4(callback_register,e4);
	thread t5(callback_register,e5);
	usleep(100);

	invokeTrigger();

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	int cnt = 0;
	while(true){
		if(cnt == 5){
			closeTrigger();
		}
		if(cnt == 10){
			invokeTrigger();
		}
		if(cnt == 15){
			break;
		}
		thread t (callback_register,e1);
		t.join();
		cnt++;
	}
	exit(1);

}
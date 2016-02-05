#include<stdio.h>
#include<stdlib.h>

/**
 * Atomic Operation fulfilled by Hardware
 * @param  p   point p that wait to be set
 * @param  old_val old value of *p
 * @param  new_val  new value that should set to p 
 * @return     if set successfully return true
 */
#define LOCKED 1

bool CAS(int *p,int old_val, int new_val){
	int tmp = *p;
	if(tmp == old){
		*p = new_val;
		return true; 
	}
	else{
		return false;
	}
}


/**
 * TestAndSet Atomic operation, only one process can perform this function at one time
 * @param  lockPtr write Locked(1) to lockPtr position
 * @return         return old value
 */
int TestAndSet(int* lockPtr) {
    int oldValue;
     
    // Start of atomic segment
    // The following statements should be interpreted as pseudocode for
    // illustrative purposes only.
    // Traditional compilation of this code will not guarantee atomicity, the
    // use of shared memory (i.e. not-cached values), protection from compiler
    // optimization, or other required properties.
    oldValue = *lockPtr;
    *lockPtr = LOCKED;
    // End of atomic segment

    return oldValue;
}


/*******Atomic openation use compare and set***********/
/**
 * Atomic Increment to value
 * @param x  value need to be increased
 */
int atomicIncre(int x){
	int tmp = x;
	while(!CAS(&x,tmp,tmp+1)){
		tmp = x;
	}
	return x;
}

/**
 * Atomic add Node to head
 * @param p     node wait to be added to head
 * @param head  original head
 */
Node * atomicAddToHead(Node *p, Node *head){
	Node * tmp = head;
	p->next = head;
	while(!CAS(head,tmp,p)){
		tmp = head;
		p->next = head;
	}
	return head;
}

/******SPIN LOCK*******/
/*Problem with SpinLock: 
	If two process are trying to enter a critical section ,there is no gurantee for FIFO
	More serious: if P may use spinlock within a loop and use the lock repeatedly and Q can't access it
		          this can be solved by checking if another process is waiting for the lock if yes then give up the lock
	Also spinlock is dangerous  to use in uniprocessor because of the priority inversion
	if lower priority processes hold the lock, while higher process enters and achieve the CPU, then deadlock happens!
Two possible solutions to the priority inversion problem:
	Execute the critical regions with masked interrupts. Thus the current process cannot be preempted (in the case we are considering there is only one processor).
	Determine the maximum priority among the processes that can execute these critical regions, then make sure that all processes execute the critical regions at this priority. 
		Since now competing processes have the same priority, they cannot preempt each other.
	
	A related problem is the formation of scheduling convoys. This occurs when a process ceases to run in 
the middle of a critical region [because of page fault, quantum expiration, external interrupt] and then all other processes sharing this region have to wait for its completion. 
A number of techniques are being introduced to deal with this problem, in particular Preemption-safe Locking and Non-blocking Algorithms. 
[Preemption-safe locking tries to avoid preemption during a critical region by having the kernel extend the quantum of a process in the critical region or by increasing its priority. 
Non-blocking algorithms, expecially for implementing shared counters and queues, make sure that if a process blocks within a critical region, 
it can be brought back to the beginning of the critical region and another process is allowed to enter the region.]

	In general spinlocks with their busy waiting mechanism are desirable (as compared to non busy waiting mechanisms like semaphores) 
/when the scheduling and context switching time are greater than the time required to execute the critical region, or when there are processors not required to do other tasks.
*/
/**
 * Non blocking spin lock by using TestandSet 
 * @param lock [description]
 */
//Problem high precentage of memory bus bandwidth, by repeatly testandseting
void initLock(int *lock){
	*lock = 0;
}
void spinLock(int *lock){
	while(TestAndSet(lock)){
		;
	}
}
void UnLock (int *lock) {
	*lock = 0;
}

//Improve
//the Inner loop is a read operation which uses less of memory bus bandwidth,
//and this read operation is normally carried out on a cached copy!!!!!
void  improvedSpinLock(int * lock){
	while(TestandSet(lock)){
		while(*lock == 1);
	}
}






/*
This is an extension of Peterson's algorithm to the N-process case. 
When presenting this algorithm, Peterson notes that there are more efficient algorithms (such as Eisenberg and McGuire's).

CONCEPT: Both status values and turn values are used. 
The status array is expanded to an integer value for each process, which is used to track that process' progress 
in scanning the status of other processes. The turn value is also expanded to an integer array. 
Its values represent the relative ordering for each pair of processes.

Peterson's 2-process solution is used repeatedly in N-1 levels to eliminate at least one process 
per level until only one remains.
 */



INITIALIZATION:

shared int flags[NUMPROCS];
shared int turn[NUMPROCS - 1];
int index;

for (index = 0; index < (NUMPROCS); index++) {

	flags[index] = -1
}


for (index = 0; index < (NUMPROCS-1); index++) {
	turn[index] = 0;
}
ENTRY PROTOCOL (for Process i):

/* repeat for all partners */
for (count = 0; count < (NUMPROCS-1); count++) {

	flags[i] = count;
	turn[count] = i;

	"wait until (for all k != i, flags[k]<count) or (turn[count] != i)"

}
EXIT PROTOCOL (for Process i):

/* tell everyone we are finished */
flags[i] = -1;
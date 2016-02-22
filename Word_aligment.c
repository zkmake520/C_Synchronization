#include<stdio.h>
#include<stdlib.h>


typedef struct mix{
   char x ;  // 1 byte
   int y ;   // 4 byte, must be word-aligned
   char z ;  // 1 byte
   int w ;   // 4 byte, must be word-aligned
}Mix;
int main(){
	printf("Struct size %lu",sizeof(Mix));
}
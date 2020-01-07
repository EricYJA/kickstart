#include <stdio.h>
#include "keywords.h"

void test_extern();
void test_static();

int main(){
	printf("kick-START%c\n", 'C');
	test_extern();
	(void)woohoo();
	return 0;
}

extern int num;
void test_extern() {
	// For function, extern is added to the begining of every function defautly.
	// For variable, extern used for declaration only and memory space is not given.
	printf("num: %d\n", num);

	//ERROR
	// num = 1;
  
	//GOOD
	num = 1;
	printf("num: %d\n", num);
}

void test_static() {
	static int fig = 1;
	// int fig = 1; 
}

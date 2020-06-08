#include <stdio.h>
#include "keywords.h"

// For extern test
int num = 10;

// For static test
// This global var could not be used by other file
// static int num = 10;

// static void woohoo();

void woohoo() {
	printf("woohoo\n");
}


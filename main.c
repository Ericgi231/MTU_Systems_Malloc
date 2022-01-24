#include "hmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*You may include any other relevant headers here.*/


/*	main()
 *	Use this function to develop tests for hmalloc. You should not 
 *	implement any of the hmalloc functionality here. That should be
 *	done in hmalloc.c
 *	This file will not be graded. When grading I will replace main 
 *	with my own implementation for testing.*/
int main(int argc, char *argv[]){
	int *test = hmalloc(sizeof(int)*8);
	char *test2 = hmalloc(sizeof(char)*24);
	int *test3 = hmalloc(sizeof(int)*4);
	char *test4 = hmalloc(sizeof(char)*12);

	hfree(test);
	hfree(test2);
	hfree(test3);

	traverse();

	int *test5 = hcalloc(sizeof(int)*7);

	test5[0] = 1;
	test5[1] = 2;
	test5[2] = 3;
	test5[3] = 4;

	printf("%d %d %d %d\n", test5[0], test5[1], test5[2], test5[3]);
	//hrealloc(test5);
	printf("%d %d %d %d\n", test5[0], test5[1], test5[2], test5[3]);

	char *test6 = hcalloc(sizeof(char)*26);
	int *test7 = hcalloc(sizeof(int)*6);
	char *test8 = hcalloc(sizeof(char)*10);

	hfree(test5);
	hfree(test6);
	hfree(test7);
	hfree(test8);
	hfree(test4);


	return 1;
}

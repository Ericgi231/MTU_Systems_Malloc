#include "hmalloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*You may include any other relevant headers here.*/


/*Add additional data structures and globals here as needed.*/
void *free_list = NULL;

/* traverse
 * Start at the free list head, visit and print the length of each
 * area in the free pool. Each entry should be printed on a new line.
 */
void traverse(){
   /* Printing format:
	 * "Index: %d, Address: %08x, Length: %d\n"
	 *    -Index is the position in the free list for the current entry. 
	 *     0 for the head and so on
	 *    -Address is the pointer to the beginning of the area.
	 *    -Length is the length in bytes of the free area.
	 */
	if (free_list == NULL) { return; }
	void *cur = free_list;
	int i = 0;
	while(1){
		printf("Index: %d, Address: %08x, Length: %d, Link: %d\n", i, (unsigned int)cur, *((int *)cur-2), *((int *)cur-1));
		//if end of list stop
		if(*((int *)cur-1) == 0){ break; }
		//go down list one level
		cur += *((int *)cur-1);
		i++;
	}
}

/* hmalloc
 * Allocation implementation.
 *    -will not allocate an initial pool from the system and will not 
 *     maintain a bin structure.
 *    -permitted to extend the program break by as many as user 
 *     requested bytes (plus length information).
 *    -keeps a single free list which will be a linked list of 
 *     previously allocated and freed memory areas.
 *    -traverses the free list to see if there is a previously freed
 *     memory area that is at least as big as bytes_to_allocate. If
 *     there is one, it is removed from the free list and returned 
 *     to the user.
 */
void *hmalloc(int bytes_to_allocate){
	//if free_list has values
	if (free_list != NULL) {
		void *cur = free_list;
		//loop until end of free list or large enough space
		void *prev = NULL;
		while(1){
			//large enough space found
			if (bytes_to_allocate <= *((int *)cur-2))
			{
				void *p = cur;
				//update size of new allocated mem
				//*((int *)p-2) = bytes_to_allocate;
				*((int *)p-1) = 0; //ADDED POST SUBMIT
				
				//if last item in free list and also head of free list
				if (*((int *)cur-1) == 0 && prev == NULL) {
					//free list is empty
					free_list = NULL;
				}
				//if item is just head of free list
				else if (prev == NULL) 
				{
					//pop head off free list
					free_list = cur + *((int *)cur-1);
				}
				//if just last item in free list
				else if (*((int *)cur-1) == 0) {
					//designate second to last item as now being last item
					*((int *)prev-1) = 0;
				} 
				//item is in middle of free list
				else 
				{
					//add current nodes offset to parent nodes offset,
					//thus making it point at current nodes child
					*((int *)prev-1) += *((int *)cur-1);
				}

				return p;
			}
			//end of free list reached
			else if (*((int *)cur-1) == 0 )
			{ 
				break;
			}
			prev = cur;
			cur += *((int *)cur-1);
		}
	} 
	//else means free list is empty, must find new space
	void *p = sbrk(bytes_to_allocate+8);
	*(int *)p = bytes_to_allocate;
	*((int *)p+1) = 0;
	return p+8;
}

/* hcalloc
 * Performs the same function as hmalloc but also clears the allocated 
 * area by setting all bytes to 0.
 */
void *hcalloc(int bytes_to_allocate){
	void *p = hmalloc(bytes_to_allocate);
	for (int i = 0; i < bytes_to_allocate; i++) {
		*((char *)p+i) = 0;
	}
	return p;
}

/* hfree
 * Responsible for returning the area (pointed to by ptr) to the free
 * pool.
 *    -simply appends the returned area to the beginning of the single
 *     free list.
 */
void hfree(void *ptr){
	//if free list empty, can set head to freed point
	if (free_list == NULL){
		free_list = ptr;
		*((int*)free_list-1) = 0; //ADDED POST SUBMIT
	}
	//else set head to new point and direct to old head 
	else 
	{
		void *tmp = free_list;
		free_list = ptr;
		*((int*)free_list-1) = tmp - free_list;
	}
}

/* For the bonus credit implement hrealloc. You will need to add a prototype
 * to hmalloc.h for your function.*/
void *hrealloc(void *ptr){
	//get size
	int bytes_to_alloc = *((int *)ptr-2);
	//get new area
	void *p = hmalloc(bytes_to_alloc);

	//copy data from old to new
	for (int i = 0; i < bytes_to_alloc; i++) {
		*((char *)p+i) = *((char *)ptr+i);
	}

	//free old area
	hfree(ptr);

	return p;
}

/*You may add additional functions as needed.*/

/*
 *File:         deque.c
 *
 *Description:  This file contains the functions needed to implement a linked list as
 *				a queue in order to complete a series of tasks
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "deque.h"
typedef struct node {
	int data;
	struct node *prev;
	struct node *next;
}NODE; 

struct deque {
	NODE *sentinel;
	int count;
};

/*
 * Function: 	createSet
 * Description: create and return a pointer to a DEQUE with a sentinel node
 * Runtime:		O(1) 
 */
DEQUE *createDeque(void) {
	DEQUE *dp = (DEQUE *)malloc(sizeof(DEQUE));
	assert(dp != NULL);
	dp->sentinel = malloc(sizeof (NODE));
	assert(dp->sentinel != NULL);
	/*Link the sentinel node to itself */
	dp->sentinel->next = dp->sentinel;
	dp->sentinel->prev = dp->sentinel;
	/*Sentinel node contains no data, so it does not count towards count*/
	dp->count = 0;
	return dp;
}//end createDeque
/*
 * Function: 	destroySet
 * Description: traverse through the linked list and free all of the memory that has been allocated
 * Runtime:		O(n), where n is the length of the list
 */
void destroyDeque(DEQUE *dp) {
	assert(dp != NULL);
	NODE * head = dp->sentinel->next;
	NODE * temp;
	/* 
	 * If dp->sentinel->next == NULL, then dp->sentinel is the only thing to free and the while loop wont run
	 * Want the while loop to go through the list and delete all nodes until it circles back and hits the sentinel, as that marks the end of the circle
	 * Free the sentinel outside of the loop and then free the deque
	*/
	while (head != NULL && head != dp->sentinel) {
		temp = head;
		head = head->next;
		free(temp);
	}//end while
	free(dp->sentinel);
	free(dp);
	return;
}//end destroyDeque

/*
 * Function: 	numItems
 * Description: return the number of items in the deque
 * Runtime:		O(1)
 */
int numItems(DEQUE * dp) {
	return dp->count;
}//end numItems

/*
 * Function: 	addFirst
 * Description: add the data into a node at the beginning of a given list.  Allocate memory for the node, and assign pointers accordingly
 * Runtime:		O(1)
 */
void addFirst (DEQUE *dp, int x) {
	assert(dp != NULL);
	/*Allocate memory for a new node and then assert that it was allocated correctly*/
	NODE * np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->data = x; 
	/* Insert the new node into the circular doubly linked list directly after the sentinel(sentinel->next = np).*/
	dp->sentinel->next->prev = np;
	np->next = dp->sentinel->next;
	np->prev = dp->sentinel;
	dp->sentinel->next = np;
	dp->count++;
}//end addFirst

/*
 * Function: 	addLast
 * Description: add the data into a node at the end of a given list.  Allocates memory for the node and assigns pointers accordingly
 * Runtime:		O(1)
 */
void addLast(DEQUE *dp, int x) {
	assert(dp != NULL);
	NODE * np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->data = x;
	/*Insert the new node after the sentinel (sentinel->prev = np)*/
	dp->sentinel->prev->next = np;
	np->prev = dp->sentinel->prev;
	np->next = dp->sentinel;
	dp->sentinel->prev = np;
	dp->count++;
}//end addLast

/*
 * Function: 	removeFirst
 * Description: returns the data of the first node in the list, reassigns pointers to keep the list together, and then removes the first node
 * Runtime:		O(1)
 */
int removeFirst(DEQUE *dp) {
	assert(dp != NULL);
	/*If the sentinel is the only node in the deque, then there is nothing to remove and return*/
	if (dp->count == 0)
		return -1;
	/*Return the data (int) from the node after sentinel (sentinel->next) and reassign pointers*/
	int data = dp->sentinel->next->data;
	NODE *np = dp->sentinel->next; //temporary copy
	dp->sentinel->next = np->next;
	np->next->prev = dp->sentinel;
	free(np);
	dp->count--;
	return data;
}//end removeFirst

/*
 * Function: 	removeLast
 * Description: returns the data of the last node in the list, reassigns pointers to keep list together and then removes the last node
 * Runtime:		O(1)
 */
int removeLast(DEQUE *dp) {
	assert(dp != NULL);
	/*If the sentinel is the only node in the deque, then there is nothing to remove and return*/
	if (dp->sentinel->next == NULL)
		return - 1;
	/*Return the data (int) from the node before sentinel (sentinel->prev) and reassign pointers*/
	int data = dp->sentinel->prev->data;
	NODE *np = dp->sentinel->prev; //temporary copy
	dp->sentinel->prev = np->prev;
	np->prev->next = dp->sentinel;
	free(np);
	dp->count--;
	return data;
}//end removeLast

/*
 * Function: 	getFirst
 * Description: return the data from the first node in the list, but does not remove it
 * Runtime:	O(1)
 */
int getFirst (DEQUE *dp) {
	assert(dp != NULL);
	if (dp->sentinel->next == NULL)
		return -1;
	return dp->sentinel->next->data;
}//end getFirst

/*
 * Function: 	getLast
 * Description: returns the data from the last node in the list, but does not remove it
 * Runtime:		O(1)
 */
 int getLast (DEQUE *dp) {
	assert(dp != NULL);
	if (dp->sentinel->next == NULL)
		return -1;
	return dp->sentinel->prev->data;
 }//end getLast

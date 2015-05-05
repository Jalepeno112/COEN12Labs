/*
 *File:			unsorted.c
 *
 *Description:	This file contains the functions need to check a text file 
 *				for the number of unique words using sequential search
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "set.h"

struct set {
	int count;
	int length;
	char **elts;
};
/*
 * Function:	createSet
 *
 * Description:	creates a pointer to a set that holds an array of strings, the number of elements in the array 
 *				and the length of the array
 */
SET *createSet(int maxElts) {
	SET *sp; 
	sp = malloc(sizeof(SET));
	/* verify malloc ran properly */
	assert(sp != NULL);
	sp->elts = malloc(sizeof(char*)*maxElts); //initialize the array of strings to hold unique words
	/* verify that array was intialized properly*/
	assert(sp->elts != NULL);
	sp->count = 0;
	sp->length = maxElts;
	
	return sp;
}//end createSet

/*
 * Function:	destroySet
 *
 * Description:	destroys the set by freeing the memory that the set holds
 *				O(n)
 */

void destroySet(SET *sp) {
	/* delete the individual strings, then delete the array, then delete the set */
	assert(sp != NULL);
	int i;
	for(i = 0; i < sp->count; i++) {
		free(sp->elts[i]);
	}//end i
	free(sp->elts);
	free(sp);
}//end destroySet

/*
 * Function:	numElements
 *
 * Description:	returns the number (int) of elements in a set
				O(1)
 */

int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->count;
}//end numElements

/*
 * Function:	findElement
 *
 * Description:	traverses through the array using sequential search to find a particual element of the array
 *				O(n)
 */

static int findElement(SET *sp, char *elt) {
	/* go through the array in sp sequentially to locate elt, return the index of elt if found, if not return -1 */
	assert(sp != NULL);
	int i;
	for(i = 0; i < sp->count; i++) {
		if (strcmp(sp->elts[i], elt) == 0) {
			return i;
		}//end if strcmp == 0
	}//end for i
	return -1; //word not found
}//end findElement

/*
 * Function:	hasElement
 *
 * Description:	uses findElement to return a boolean value for whether or not an element is in the set
 *				O(n)
 */

bool hasElement(SET *sp, char*elt) {
	assert(sp != NULL);
	/* if findElement returns something other than -1, then the element is in the set, return true, else return false */
	if(findElement(sp, elt) != -1) {
		return true;
	}//end if
	else {
		return false;
	}
}//end hasElement

/*
 * Function:	addElement
 *
 * Description:	Add an element to the array if it isn't already in the array
 *				O(n)
 */

bool addElement(SET *sp, char *elt) {
	assert(sp != NULL);
	/* If the set does not contain elt, then add it to the set using strdup to allocate memory and then increment sp->count */
	if(hasElement(sp, elt) == false) {
		//check to make sure set has space
		assert(sp->count < sp->length);
		//add element to the end of the array
		sp->elts[sp->count] = strdup(elt);
		//increase count by 1
		sp->count = sp->count + 1;
		return true;
	}//end if
	else {
		return false;
	}//end else
}//end addElement

/*
 * Function:	removeElement
 *
 * Description:	removes a given element from a set
 *				O(n)
 */

bool removeElement(SET *sp, char *elt) {
	int index = 0;
	/* if elt can be found, then move the final element in the array to the index of elt, and free the final index */
	if((index = findElement(sp, elt)) != -1) {
		//[a][b][c][d][e]
		//remove 2 (element c)
		//copy 4 to 2
		//[a][b][e][d][e]
		//delete the last index casue now it appears twice
		//[a][b][e][d][]
		sp->elts[index] = strdup(sp->elts[sp->count-1]);
		free(sp->elts[sp->count-1]);
		sp->count -= 1;
		return true;
	}//end if
	else {
		return false;
	}//end else
}//end removeElement

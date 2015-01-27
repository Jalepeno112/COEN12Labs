/*
 *File:         sorted.c
 *
 *Description:  This file contains the functions need to check a text file
 *              for the number of unique words using binary search
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#define EMPTY 0
#define FILLED 1
#define DELETED 2
struct set {
    int count;
    int length;
    char **elts;
	char *flags;
};

/*
 * Function:    createSet
 *
 * Description: creates a pointer to a set that holds an array of strings, the number of elements in the array
 *              and the length of the array
 *				O(1)
 */
SET *createSet(int maxElts) {
        SET *sp;
        sp = malloc(sizeof(SET));
        /* verify malloc ran properly */
        assert(sp != NULL);
        sp->elts = malloc(sizeof(char*)*maxElts); 
        /* verify that array was intialized properly*/
        assert(sp->elts != NULL);
		sp->count = 0;
        sp->length = maxElts;
		sp->flags = malloc(sizeof(char) *maxElts);
		assert(sp->flags != NULL);
		int i;
		for (i = 0; i < sp->length; i++) {
			sp->flags[i] = EMPTY;
		}//end for i
        return sp;
}//end createSet

/*
 * Function:    destroySet
 *
 * Description: destroys the set by freeing the memory that the set holds
 *              O(n)
 */

void destroySet(SET *sp) {
        /* delete the individual strings, then delete the array, then delete the set */
        assert(sp != NULL);
        int i;
        for(i = 0; i < sp->length; i++) {
			if (sp->flags[i] != DELETED) {
				free(sp->elts[i]);
			}
        }//end i
        free(sp->elts);
		free(sp->flags);
        free(sp);
}//end destroySet

/*
 * Function:    numElements
 *
 * Description: returns the number (int) of elements in a set
 *              O(1)
 */

int numElements(SET *sp) {
        assert(sp != NULL);
        return sp->count;
}//end numElements

/*
 * Function: hashString
 *
 * Description: returns the value of the hash function for a given string
 *				O(n) where n is the strings length
*/

 unsigned hashString(char *s) {
	unsigned hash = 0;
	while(*s != '\0')
		hash = 31 * hash + *s ++;
	return hash;
 }//end hashString

 /*
 * Function:    findElement
 *
 * Description: traverses through the array using hashing to find the index of an element
 *              Best Case: O(1)
 *				Expected case: O(1)
 *				Worst case: O(n)
 */
static int findElement(SET *sp, char *elt, bool *found) {
	assert(sp != NULL);
	int index = (hashString(elt))%(sp->length);
	
	/* If the set does not contain elt, then set found == false, and return the index */
	if (sp->flags[index] == EMPTY) {
		*found = false;
		return index;
	}//end if
	int firstDeleted = -1;
	
	/* if the index is not empty (filled, or deleted), then search to see if the element was inserted further down the hash table*/
	while (sp->flags[index] != EMPTY) {
		if(strcmp(sp->elts[index], elt) == 0 && sp->flags[index] != DELETED) {
			*found = true;
			return index;
		}
		/* record the first deleted entry found in order to reclaim it later (only want to find it once) */
		if (sp->flags[index] == DELETED && firstDeleted == -1) {
			firstDeleted = index;
		}
		index = (index + 1) % (sp->length);
	}//end while
	
	/*
 	* If the while loop terminates, then it has found an empty slot, and that is where the element should be inserted, or the element cannot be found
	* Want to clear some of the deleted entries, so set index to the first deleted entry in the hash table	if a deleted entry was encountered
	*/
		
	*found = false;
	if (firstDeleted != -1) {
		index = firstDeleted;
	}
	return index;
}//end findElement

/*
 * Function:    hasElement
 *
 * Description: uses findElement to return a boolean value for whether or not an element is in the set
 *				Best case : O(1)
				Worst case:  O(n)
 *				Expected case: O(1)
 */
 
bool hasElement(SET *sp, char*elt) {
        assert(sp != NULL);
        /* if findElement sets *found to false, then elt is not in the set, return false, else return true*/
		bool found;
		int index;
		index = findElement(sp,elt, &found);   
        if(found == false) {
            return false;
        }//end if
        else {
            return true;
        }
}//end hasElement

/*
 * Function:    addElement
 *
 * Description: Add an element to the array if it isn't already in the array
 *              Best case: O(1)
 *				Expected Case: O(1)
 *				Worst case: O(n)
 */

bool addElement(SET *sp, char *elt) {
	assert(sp->count < sp->length);
	bool found;
	/*	If sp->length == sp->count, then the table is full and nothing more can be added*/
	if (sp->length == sp->count) {
		return false;
	}
	int index = findElement(sp, elt, &found);
	/*If findElement returns false, then elt is not a duplicate and can be added at the index*/
	if (found == false) {
		sp->elts[index] = strdup(elt);
		sp->flags[index] = FILLED;
		sp->count++;
		return true;
	}//end if
	else {
		return false;
	}
}//end addElement

/*
 * Function:    removeElement
 *
 * Description: removes a given element from a set
 *              Best case: O(1)
 *				Expected case: O(1)
 *				Worst case : O(n)
 */

bool removeElement(SET *sp, char *elt) {
        int index = 0;
		bool found;
        /* if elt can be found, then move the final element in the array to the index of elt*/
		index = findElement(sp, elt, &found);
        if(found == true) {
			free(sp->elts[index]);
			sp->flags[index] = DELETED;
			sp->count--;
			return true;
        }//end if
        else {
            return false;
        }//end else
}//end removeElement
/*
 *Name:			Giovanni Briggs
 *Date:			4/19/13
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
struct set {
        int count;	//number of elements in the set
        int length;	//maximum number of elements in the set (how many elements it can hold)
        char **elts;	//a dynamically allocated array of strings
};

/*
 * Function:    createSet
 *
 * Description: creates a pointer to a set that holds an array of strings, the number of elements in the array
 *                              and the length of the array
 */
SET *createSet(int maxElts) {
        SET *sp;
        sp = malloc(sizeof(SET));
        /* verify malloc ran properly */
        assert(sp != NULL);
        sp->elts = malloc(sizeof(char*)*maxElts); //initialize the array of strings to hold unique words
        /* verify that array was intialized properly*/
        assert(sp->elts != NULL);
        //set the count of the set = 0 becaue there are no elements in the set
	sp->count = 0;
	//set the length of the set to maxElts because that is the total number of elements that can exist in the set
        sp->length = maxElts;
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
        for(i = 0; i < sp->count; i++) {
            free(sp->elts[i]);
        }//end i
        free(sp->elts);
        free(sp);
}//end destroySet

/*
 * Function:    numElements
 *
 * Description: returns the number (int) of elements in a set
                O(1)
 */

int numElements(SET *sp) {
        assert(sp != NULL);
        return sp->count;
}//end numElements
/*
 * Function:    findElement
 *
 * Description: traverses through the array using binary search to find the index of an element
 *              O(logn)
 */

static int findElement(SET *sp, char *elt, bool *found) {
	int lo, hi, mid;
	lo = 0;
	hi = sp->count-1;
	assert(elt != NULL);
	//printf("sp->count == %d\n", sp->count);
	while (lo <= hi) {
		mid = (lo + hi)/2;
		/*elt is lower than mid, then decrease the high boundry*/
		if(strcmp(elt, sp->elts[mid]) < 0) {
			hi = mid - 1;
		}
		/*elt is higher than mid, then incrase the low boundry*/	
		else if (strcmp(elt,sp->elts[mid]) > 0)
			lo = mid + 1;
		/*if elt == mid, then return mid and set found to true*/
		else {
			*found = true;
			return mid;
		}
	}//end while
	*found = false;
	return lo;
}//end findElement

/*
 * Function:    shiftup
 *
 * Description: moves all of the elements in the array past a given index by one and then inserts a new element 
 *				in that index so that the array stays sorted.  Used with addElement
 *              O(n)
 */

static bool shiftup(SET *sp, char *elt, int index){
	/* set i to the last occupied index+1, and then shift every entry in the array up one index and finish by inserting elt into the newly opened index	*/
	int i = 0;
	for (i = sp->count; i > index; i--) {
		sp->elts[i] = sp->elts[i-1];
	}//end for i
	/* allocte memory for string and then copy that string to sp->elts[index] using strdup*/
	sp->elts[index] = strdup(elt);	
	return true;
}

/*
 * Function:    shiftdown
 *
 * Description: moves all of the elements in the array past a given index by -1 and then inserts a new element 
 *				in that index so that the array stays sorted. Used with removeElement
 *              O(n)
 */
 
static bool shiftdown(SET *sp, char *elt, int index) {
	int i = 0;
	/* free the element indicated bu index and then shift all of the elements down by 1 */
	free(sp->elts[index]);
	for (i = index+1; i < sp->count; i++) {
		sp->elts[i-1] = sp->elts[i];
	}//end for i
	
	return true;
}//end shiftdown

/*
 * Function:    hasElement
 *
 * Description: uses findElement to return a boolean value for whether or not an element is in the set
 *              O(logn)
 */
 
bool hasElement(SET *sp, char*elt) {
        assert(sp != NULL);
        /* if findElement sets *found to false, then elt is not in the set, return false, else return true*/
		bool f = true;
		int index;
		index = findElement(sp,elt, &f);	//binary search to find element
        if(f == false) {
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
 *              O(n)
 */

bool addElement(SET *sp, char *elt) {
        assert(sp != NULL);
        /* If the set does not contain elt, then add it to the set using strdup to allocate memory and then increment sp->count */
	int index = 0;
	bool f = false;
	/*
 	 * find element takes three parameters, the set, the element that you are looking for, and a boolean that is passed by reference
 	 * After findElement runs, the boolean that is passed (in this case f) will either equal true or false.  
	 */
	index = findElement(sp, elt, &f);
	/* if found is false, then elt is not in the set, so findElement will return lo, which is the position where elt needs to be added so that the set remains sorted*/
        if(f == false) {
		//make sure that the set has space
		assert(sp->count < sp->length);
		//shift every element up 1 index in order to insert the new element at the front of the set
		shiftup(sp,elt,index);
		sp->count++;
        }//end if
	//if found is true, then the element already exists and does not need to be added
        else {
            return false;
        }//end else
}//end addElement

/*
 * Function:    removeElement
 *
 * Description: removes a given element from a set
 *              O(n)
 */

bool removeElement(SET *sp, char *elt) {
        int index = 0;
	bool f = true;
        /* if elt can be found, then the element needs to be removed from the set, and everything past it needs to be shifted down so that there is no empty spot in the middle of the set*/
	index = findElement(sp, elt, &f);
        if(f == true) {
            	shiftdown(sp, elt, index);
		sp->count--;
		int i;
		return true;
        }//end if
        else {
            return false;
        }//end else
}//end removeElement

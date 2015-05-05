/*
 *File:         radix.c
 *
 *Description:	Utilizes deque.c to implement a radix sort to sort a series of integers
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "deque.h"

void sort(DEQUE **dp, DEQUE *lp, int digit);

int main(void) {
	DEQUE* storage[10];
	DEQUE* input = createDeque();
	int i;
	
	/* Create the deques that storage will hold*/
	for (i = 0; i < 10; i++) {
		storage[i] = createDeque();
	}//end for
	
	/*Recieve input data, place them in the right queue, and determine the greatest number of digits*/
	int max = 0;
	int x;
	while(scanf("%d", &x) == 1) {
		if (x > max)
			max = x;
		/*Last in first out, so add new elements to the front and then take elements out from the rear (removeLast())*/
		addFirst(input, x);		
	}//end while
	
	/*Rutime is the number of times that the program has to run in order to sort through all digit places of every integer*/
	int runtime = (int)(log(max + 1)/log(10));
	int count = numItems(input);
	
	/*
	 * Go through the deques and sort them back into the input list
	 * Use digit to keep track of what digit place we are sorting
	 */
	int digit = 0;
	for (i = 0; i <= runtime; i++) {
		sort (storage,input, digit);
		digit++;
	}//end for i
	
	/* Once the sort has finished, everything should be in the first deque (storage[0]).  Sort that back to the input deque and print */
	for (i = 0; i < count; i++) {
		int x = removeLast(input);
		printf("%d ", x);
	}
	
	/*Free the standalone input deque, then go and free each deque in the array*/
	destroyDeque(input);
	for (i = 0; i < 10; i++) {
		destroyDeque(storage[i]);
	}
	return 0;
}//end main

/*
 * Function: sort
 *
 * Description: loop through the input deque and sort the data into their individual bins in array of deques, then go and copy the sorted material
 *				back into the input deque.  Repeat as necessary.
 * Rutime:		O(n)
 */
void sort(DEQUE **dp, DEQUE *lp, int digit) {
	/*dp = the pointer to the array of deques; lp = the pointer to the input deque, and digit is the digit place that we are sorting*/
	int i, j;
	/*Take the data out of the input deque and put them in their respective bins (storage[i])*/
	int count = numItems(lp);
	int power = pow(10, digit);
	for(i = 0; i <count; i++) {
		int x = removeLast(lp);
		/*the program is only allowed to let positive data through, so check to make sure data is positive*/
		if (x >= 0) {
			int index;
			index =(x/(power))%10;
			addFirst(dp[index], x);
		}//end if x > =0
	}//end for i
	
	/*For each of the ten deques in the array (dp), go through and add them back to the input deque*/
	for (i = 0; i < 10; i++) {
		int count = numItems(dp[i]);
		for (j = 0; j < count; j++) {
			int x = removeLast(dp[i]);
			if (x >= 0) {
				addFirst(lp, x);
			}//end if
		}//end j
	}//end for i
}//end sort


/*
 * File:        bincount.c
 *
 * Description: This file contains the main function for testing a set
 *              abstract data type for strings.
 *
 *              The program takes a single file as a command line argument.
 *              An array of sets is used to count the number of words of
 *              varying lengths.  The counts of the number of distinct
 *              words of each length are printed.
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "set.h"


/* This is sufficient for the test cases in /scratch/coen12. */

# define MAX_UNIQUE 18000
# define MAX_WORD_LENGTH 30
# define MAX_DISPLAYED 20


/*
 * Function:    main
 *
 * Description: Driver function for the test application.
 */

int main(int argc, char *argv[])
{
	/*
	 * The point of this file is to display the number of unique words of VARIOUS SIZES
	 * Works with both UNSORTED.C and SORTED.C
	*/
    FILE *fp;
    char buffer[MAX_WORD_LENGTH];
	//create a 2D array of sets.  Each index of the sets belogs to a certain word length (indexes vary from 0 to MAX_WORD_LENGTH-1).  And at each index, there is a set containing all of the words of that word length	
    SET *sets[MAX_WORD_LENGTH];
    int i;


    /* Check usage and open the file. */

    if (argc != 2) {
        fprintf(stderr, "usage: %s file1 [file2]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "%s: cannot open %s\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }


    /* Insert all words into the set of the appropriate length. */

    for (i = 0; i < MAX_WORD_LENGTH; i ++)
	sets[i] = createSet(MAX_UNIQUE);

    while (fscanf(fp, "%s", buffer) == 1)
	//add the element to the set at the index of strlen-1. I believe I did this because the space or punctuation after the word is included when you do fscanf.  We dont want any of that to count. So that is why you do strlen - 1.
        addElement(sets[strlen(buffer) - 1], buffer);

    fclose(fp);


    /* Display the counts for each word length. */

    for (i = 0; i < MAX_DISPLAYED; i ++) {
	printf("%5d distinct words ", numElements(sets[i]));
	printf("of length %d\n", i + 1);
    }

    exit(EXIT_SUCCESS);
}

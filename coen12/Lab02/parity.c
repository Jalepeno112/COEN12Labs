/*
 * File:        parity.c
 *
 * Description: This file contains the main function for testing a set
 *              abstract data type for strings.
 *
 *              The program takes a single file as a command line argument.
 *              A set is used to maintain a collection of words that occur
 *              an odd number of times.  The counts of total words and
 *              words appearing an odd number of times are printed.
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "set.h"


/* This is sufficient for the test cases in /scratch/coen12. */

# define MAX_SIZE 18000


/*
 * Function:    main
 *
 * Description: Driver function for the test application.
 */

int main(int argc, char *argv[])
{
	/*
	 * This file is designed to display the number of words that occur an odd number of times.  It's really stupid.
	 */
    FILE *fp;
    char buffer[BUFSIZ];
    SET *odd;
    int words;


    /* Check usage and open the file. */

    if (argc != 2) {
        fprintf(stderr, "usage: %s file1\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "%s: cannot open %s\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }


    /* Insert or delete words to compute their parity. */

    words = 0;				//total number of words
    odd = createSet(MAX_SIZE);		//the set that contains all words that appear an odd number of times

    while (fscanf(fp, "%s", buffer) == 1) {
        words ++;
	
	/*
	 * If the word is unique, then you add the element, and that counts as appearing 1 time.  1 is odd
	 * Therefore, the next time you see that word, that is the 2nd time you have seen it, which means that it occurs an even number of times
	 * So, you have to remove it from the list.
	 * Algorithm: check if the word is already in the set, if not add it (it's been seen an odd number of times), it it is in the set, remove it (its been seen an even number of times)
	 */
        if (hasElement(odd, buffer))
            removeElement(odd, buffer);
        else
            addElement(odd, buffer);
    }

    printf("%d total words\n", words);
    printf("%d words occur an odd number of times\n", numElements(odd));
    fclose(fp);

    destroySet(odd);	//free all memory
    exit(EXIT_SUCCESS);
}

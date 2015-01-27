#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "tree.h"
#define SIZE 256
#define p(x) (((x)-1)/2)
#define l(x) (((x)*2)+1)
#define r(x) (((x)*2)+2)
//FUNCTIONS
void count(FILE *file, int letterCount[]);
struct tree* insert(struct tree *newTree);
struct tree* buildTree();
void printHuffmanTree(struct tree *t);
struct tree *extractMinTree();
void storeLeaves (struct tree *t, struct tree*leaves[]);
void heapify(int i);

void printHeap();
//GLOBALS
struct tree *pQ[SIZE+1];
struct tree *leaves[SIZE+1];
int numElements;

int main (int argc, char *argv[]) {
	if (argc != 3) {
		printf("Filenames missing, or too many filenames give!\n");
		return 1;
	}
	int i;
	numElements = 0;
	int letterCount[SIZE+1] = {};
	//set the all the spots in the leaves array to NULL
	for (i = 0; i < SIZE + 1; i++) {
		leaves[i] = NULL;
	}
	FILE *file = fopen(argv[1], "r");
	//count the number of unique characters and their number of occurances
	count(file, letterCount);
	fclose(file);
	/*
	 * for each unique character in the file, create a tree with the
	 * number of occurances of that character as the data for that tree
 	 */
	//insert the EOF tree into the priority queue seperately because its special
	insert(leaves[256]);
	for (i = 0; i < SIZE+1; i++) {
		if (letterCount[i] != 0) {
			struct tree *newTree = createTree(letterCount[i], NULL, NULL);
			leaves[i] = newTree;
			insert(newTree);
            printHeap();
		}//end if
	}//end for
	//build the huffman tree
	struct tree* huffmanTree = buildTree();
	printf("%d \n", getData(huffmanTree));
	for (i = 0; i < SIZE+1; i++) {
		if (leaves[i] != NULL) {
			//print the character, and its number of occurances
			if (isprint(i)) 
				printf("'%c': \t%d \t", i, getData(leaves[i]));
			else
				printf(" %o: \t%d \t", i, getData(leaves[i]));
			printHuffmanTree(leaves[i]);
			printf("\n");
		}//end if leaves ==NULL
	}//end for
	pack(argv[1], argv[2], leaves);
	destroyTree(huffmanTree);
	return 0;
}//end main

void printHeap() {
    int i = 0;
    printf("HEAP :\n");
    for (i = 0; i < numElements; i++){
        printf("%d ",getData(pQ[i]));
    }
    printf("\n");
}

/*
 *Funtion:	count
 *
 *Description:	goes through the designated text file and counts the number of occurances of each unique character
 */

void count (FILE *file, int letterCount[]) {
	char c;
	//get the counts for all the unique characters in the file.  Each array index is the ASCII value for the corresponding character
	while((c=getc(file)) != EOF) {
		letterCount[c]++;
	}//end while
    
	//create EOF tree and add it to the end of leaves
	struct tree *t = createTree(0, NULL, NULL);
    leaves[256] = t;
}//end count

/*
 *Funtion:		insert
 *
 *Description:	place a new tree into the priority queue at the specified index
 */
struct tree* insert(struct tree* newTree) {
	/*
	 * Insert the new node at the end of the priority queue
	 * if the new nodes parent is greater than the new, node, swap the new node and the parent
	 * continue to swap until the new node finds its place
	 */
	int index = numElements;
	pQ[index] = newTree;
	int parentIndex = p(index);
	int data = getData(newTree);
	while (parentIndex > 0) {
		//if the parent's data is less than its child, swap the child and the parent
		if (getData(pQ[parentIndex]) > data) {
			struct tree *temp = pQ[index];
			pQ[index] = pQ[parentIndex];
			pQ[parentIndex] = temp;
			index = parentIndex;
		}
		else
			break;
		parentIndex = p(index);
	}//end while
    
	//increase numElements by 1
	numElements++;
	return pQ[index];
}//end insert

/*
 *Funtion:		buildTree
 *
 *Description:	takes the priority queue and builds a huffman tree from it
 */
struct tree* buildTree() {
	/*
	 * Find the two smallest values in the priority queue, 
	 * and then create a new tree with the combined data from the two minimum nodes
	 * and set the left and right nodes of the new tree to the two minimum nodes	
	 */
	printf("Heap initial: \n");
	int i;
	for (i = 0; i < numElements; i++) {
		printf("%d ", getData(pQ[i]));
	}//end for
	printf("\n");
	while (numElements > 1) {
		int i;
		int index = 0;
		struct tree* min = extractMinTree();
		//find the second minimum value in the 
		struct tree* secondMin = extractMinTree();
		int combinedData = getData(min) + getData(secondMin);
		struct tree * combinedMin = createTree(combinedData, min, secondMin);
		insert(combinedMin);
		printf("Inserting: %d \n", getData(combinedMin));
		for( i = 0; i < numElements; i++) {
			printf("%d ", getData(pQ[i]));		
		}//end for
		printf("\n \n");		
	}//end while
	// after the loop finishes, pQ[0] should be the huffman tree
	return pQ[0];
}//end buildTree

/*
 *Funtion:		printHuffmanTree
 *
 *Description:	traverses up the tree to create and then print the binary representation of a given leaf
 */
void printHuffmanTree(struct tree *t) {
	/*
	 *If the node has no parent then it is the root, so stop and return
	 * Otherwise, if this node is the left node of its parent, print 0.
	 * If its the right node, print 1
	 */
	if(getParent(t) == NULL) {
		return;
	}
	if(getLeft(getParent(t)) == t)
			printf("0");
	if(getRight(getParent(t)) == t)
			printf("1");
	t = getParent(t);
	printHuffmanTree(t);
}//end printHuffmanTree
/*
 *Funtion:		extractMinTree
 *
 *Description:	extract the first element from the priority queue and heapify the queue
 */
struct tree* extractMinTree() {
	if (numElements == 0)
		return NULL;
	/*
	 * If the priority queue is kept in heap order, 
	 * then the first element will always be the smallest
	 */
	struct tree * extracted;
	extracted = pQ[0];
	//take the last element and insert it in the front, and decrease numElements
	pQ[0] = pQ[numElements-1];
	numElements--;
	/*after moving the last element to the front, make sure the heap is kept sorted as a min heap*/
	heapify(0);
	int i;
	printf("\n Extracting: %d \n", getData(extracted));
	for(i = 0; i < numElements; i++) {
		printf("%d ", getData(pQ[i]));		
	}//end for
	printf("\n");
	return extracted;
}//end extractMinTree

/*
 *Funtion:		heapify
 *
 *Description:	keeps the priority queue heap sorted, with the minimum value in the first slot of the array
 *				used after extracting the minimum element from the heap to make sure that shifting the heap
 *				did not break the min heap property
 */
void heapify(int i) {
	int j;
	/*printf("\nBefore heapifying! \n");	
	for(j = 0; j < numElements; j++) {
		printf("%d ", getData(pQ[j]));	
	}*/
	int l, r, smallest;
	l = l(i); //get the left node of this index
	r= r(i);  //get the right node
	/*find the samllest value between the parent and its subtrees*/
	if(l < numElements && getData(pQ[l]) < getData(pQ[i])) {
		smallest = l;	
	}
	else {
		smallest = i;	
	}
	if (r< numElements && getData(pQ[r]) <= getData(pQ[smallest])) {
		smallest = r;	
	}
	/*swap the smallest child with the parent to keep the heap sorted*/
	if(smallest != i) {
		struct tree *t = pQ[i];
		pQ[i] = pQ[smallest];
		pQ[smallest] = t;
		/*check to make sure that the heap is still sorted after moving it to its new index*/
		heapify(smallest);
	}//end if
	/*printf("\nAfter heapifying! \n");
		for(j = 0; j < numElements; j++) {
		printf("%d ", getData(pQ[j]));	
	}
	printf("\n--------------------------------------\n");
	*/	
	return;
}//end heapify

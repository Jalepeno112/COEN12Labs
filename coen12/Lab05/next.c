#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "tree.h"
#define SIZE 256
#define p(x) (((x)-1)/2)
#define l(x) (((x)*2)+1)
#define r(x) (((x)*2)+2)
//GLOBALS
struct tree *pQ[SIZE+1];
int numElements = 0;
//FUNCTIONS
int count(FILE *file, int letterCount[]);
struct tree* insert(int index, struct tree *newTree);
struct tree* buildTree();
void printTree(struct tree *t);
void printHuffmanTree(struct tree *t, int s;
struct tree *extractMinTree();
void storeLeaves (struct tree *t, struct tree*leaves[]);
void heapify(int i);

int main (int argc, char *argv[]) {
	if (argc < 0) {
		printf("Filenames missing, or too many filenames give!\n");
		return 1;
	}
	int letterCount[SIZE+1] = {};
	FILE *file = fopen(argv[1], "r");
	count(file, letterCount);
	fclose(file);
	int i;
	//int length counts the number of unique occurences
	/*
	 * for each unique character in the file, 
	 * create a tree with the number of occurances of that character as the data for that tree
 	 */
	for (i = 0; i < SIZE+1; i++) {
		if (letterCount[i] != 0) {
			struct tree *newTree = createTree(letterCount[i], NULL, NULL);
			setLetter(newTree, i);
			insert(numElements, newTree);
		}//end if
	}//end for
	//add EOF with data = 0
	struct tree *eofTree = createTree(0, NULL,NULL);
	setLetter(eofTree, EOF);
	insert(numElements,eofTree);
	//build the huffman tree
	struct tree* huffmanTree = buildTree();
	int s = 0;
	printHuffmanTree(huffmanTree, s);
	//construct leaves array for pack
	struct tree* leaves[SIZE+1];
	for (i = 0; i < SIZE + 1; i++) {
		leaves[i] = NULL;	
	}
	storeLeaves(huffmanTree, leaves);
	pack(argv[1], argv[2], leaves);
	return 0;
}//end main

int count (FILE *file, int letterCount[]) {
	char c;
	//get all of the unique characters 
	while((c=getc(file)) != EOF) {
		int index = c;
		letterCount[index]++;
	}//end while
}//end count


struct tree* insert(int index, struct tree* newTree) {
	pQ[index] = newTree;
	/*
	 * Insert the new node at the end of the priority queue
	 * if the new nodes parent is greater than the new, node, swap the new node and the parent
	 * continue to swap until the new node finds its place
	 */
	int parentIndex = p(index);
	int data = getData(newTree);
	while (parentIndex >= 0) {
		//if the parent's data is less than its child, swap the child and the parent
		if (getData(pQ[parentIndex]) > data) {
			struct tree *temp = pQ[index];
			pQ[index] = pQ[parentIndex];
			pQ[parentIndex] = temp;
			index = parentIndex;
		}
		else {
			break;
		}
		parentIndex = p(index);
	}//end while
	//increase numElements by 1
	numElements++;
	return pQ[index];
}//end insert

struct tree* buildTree() {
	/*
	 * Find the two smallest values in the priority queue, 
	 * and then create a new tree with the combined data from the two minimum nodes
	 * and set the left and right nodes of the new tree to the two minimum nodes	
	 */
	while (numElements > 1) {
		int i;
		int index = 0;
		struct tree* min = extractMinTree(pQ, numElements);
		//find the second minimum value in the 
		struct tree* secondMin = extractMinTree(pQ, numElements);
		int combinedData = getData(min) + getData(secondMin);
		struct tree * combinedMin = createTree(combinedData, min, secondMin);
		insert(numElements, combinedMin);		
	}//end while
	/*After the loop finishes, the huffman tree should be the only tree in the heap, 
	 *and it should be at index 0
	 */
	return pQ[0];
}//end buildTree
struct tree* extractMinTree() {
	if (numElements == 0)
		return;
	/*If the priority queue is kept in heap order, 
	 *then the first element will always be the smallest
	 */
	struct tree * extracted;
	extracted = pQ[0];
	//take the last element and insert it in the front, and decrease numElements
	pQ[0] = pQ[numElements-1];
	numElements--;
	/*after moving the last element to the front, check to make sure that the heap is sorted*/
	heapify(0);
	return extracted;
}//end extractMinTree

void printHuffmanTree(struct tree *t, int s) {
	/*
	 * nodes in a huffman tree either have two children or no children, 
	 * so if it has no children, then it is a leaf
	 * If it is a leaf, print out the leafs data, its character, and its binary representation
   	 * If it is not a leaf, then continue down the tree and add a 1 to the 
	 * binary represtation if going down the right tree
	 * and a 0 if going down the left tree
 	 */
	if(s ==1)
		printf("%d", 1);
	else
		printf("%d", 0);
	if (getLeft(t) == NULL && getRight(t) == NULL) {
		if (isprint(getLetter(t)))
			printf("'%c': \t%d \t", getLetter(t), getData(t));
		else
			printf(" %d: \t%d \t", getLetter(t), getData(t));
		return;	
	}//end getLeft getRight NULL
	printHuffmanTree(getRight(t), 1);
	printHuffmanTree(getLeft(t), 0);		
}//end printHuffmanTree
void storeLeaves(struct tree *t, struct tree*leaves[]) {
	//if the node is a leaf, then it is storing a character.  So place that node in leaves[c]
	if (getLeft(t) == NULL && getRight(t) == NULL) {
		int i;
		//make special exception for EOF character
		if (getLetter(t) == EOF) {
			leaves[256] = t;		
		}
		//insert the tree  into leaves[c]
		leaves[getLetter(t)] = t;
		return;
	}//end if getLeft getRight NULL
	storeLeaves(getRight(t), leaves);
	storeLeaves(getLeft(t),leaves);
}
void heapify(int i) {
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
	if (r< numElements && getData(pQ[r]) < getData(pQ[smallest])) {
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
	return;
}//end heapify

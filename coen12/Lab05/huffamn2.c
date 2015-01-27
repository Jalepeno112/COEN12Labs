#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<ctype.h>
#include "tree.h"

#define SIZE 256
#define p(x) (((x)-1)/2)
#define l(x) (((x)*2)+1)
#define r(x) (((x)*2)+2)

//FUNCTIONS
void count (FILE*, int[]);
struct tree* insert(struct tree*);
void printHuffmanTree(struct tree *t);
struct tree *extractMinTree();
void storeLeaves (struct tree*, struct tree*[]);
void heapify(int);



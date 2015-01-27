#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

struct tree{
	int data;
	struct tree *left;
	struct tree *right;
	struct tree *parent;
};

/*
 * Function: 	createTree
 *
 * Description: allocates memory for a new binary tree with specific data, left subtree and right   
 * 		subtree
 * Runtime:		O(1)
 */
struct tree *createTree(int data, struct tree *left, struct tree *right) {
	struct tree *newTree = malloc(sizeof(struct tree));
	assert(newTree != NULL);
	/*
	 * if the left or right tree is not NULL, check to see if they already have a parent node,
	 * if they already have a parent node, set their parent's left/right pointer(s) to NULL
         * then assign the data to newTree->data, and point newTree to the left and right trees
	 * and point left and right trees back to newTree
	 */
	if (left != NULL) {
		if(getParent(left) != NULL) {
			setLeft(getParent(left), NULL);
			left->parent = newTree;
		}
	}//end if left
	if (right != NULL) {
		if(getParent(right) != NULL) {
			setRight(getParent(right), NULL);
			right->parent = newTree;		
		}
	}
	newTree->data = data;
	newTree->parent = NULL;
	setLeft(newTree, left);
	setRight(newTree, right);
	return newTree;
}//end createTree

/*
 * Function: 	destroyTree
 *
 * Description: de-allocates memory of a binary tree using postorder traversal
 * Runtime:		O(nlogn)
 */
void destroyTree(struct tree *root) {
	/*Postorder.  Delete free left tree, free right tree, free the root*/
	if(root == NULL)
		return;
	destroyTree(root->left);
	destroyTree(root->right);
	free(root);
	
}//end destroyTree
/*
 * Function: 	getData
 *
 * Description: returns the data of a given node
 * Runtime:		O(1)
 */
int getData(struct tree *root) {
	assert(root!=NULL);
	return root->data;
	
}//end getData

/*
 * Function: 	getLeft
 *
 * Description: returns a pointer to the left subtree of a given root
 * Runtime:		O(1)
 */
struct tree *getLeft(struct tree *root) {	
	assert(root != NULL);
	return root->left;
}

/*
 * Function: 	getData
 *
 * Description: returns a pointer to the right subtree of a given root
 * Runtime:		O(1)
 */
struct tree *getRight(struct tree *root) {
	assert(root != NULL);
	return root->right;
}

/*
 * Function: 	getData
 *
 * Description: returns a pointer to the parent node of a given root
 * Runtime:		O(1)
 */
struct tree *getParent(struct tree *root) {
	assert(root != NULL);
	return root->parent;
}
/*
 * Function: 	setLeft
 *
 * Description: sets the left tree of the root to left, and if left is not NULL, then set the left's parent pointer to root
 * Runtime:		O(1)
 */
void setLeft(struct tree *root, struct tree *left) {
	assert(root != NULL);
	root->left = left;
	//if the left node is not NULL, the assign the left's parent pointer to root
	if (left != NULL)  
		left->parent = root;
}
/*
 * Function: 	setRight
 *
 * Description: sets the right tree of the root to right, and it right is not NULL, then set the right's parent pointer to root
 * Runtime:		O(1)
 */
void setRight (struct tree *root, struct tree *right) {
	assert(root != NULL);
	//set the right subtree of root = to right; if right isnt NULL, then set right's parent to root
	root->right = right;
	if (right != NULL)
		right->parent = root;
}

#include <iostream>
#include "deque.h"
#include <cassert>

using namespace std;

class Node{
	public:
		int data;
		Node *next, *prev;
		Node();
 };
/*
 * Function:	Node
 *
 * Description:	constructor for a node to insert into the deque
 * Runtime:		O(1)
 */

Node::Node() {
	data;
	next = NULL;
	prev = NULL;
}//end constructor
/*
 * Function:	Deque
 *
 * Description:	constructor for a deque to hold nodes and create a circular doubly linked list
 * Runtime:		O(1)
 */
Deque::Deque() {
	count = 0;
	sentinel = new Node;
	sentinel->next = sentinel;
	sentinel->prev = sentinel;
}//end constructor

/*
 * Function:	~Deque
 *
 * Description:	destructor for a deque.  Delete each node in the list and then delete the deque
 * Runtime:		O(n)
 */
Deque::~Deque() {
	Node* head = this->sentinel->next;
	Node *temp;
	/* 
	 * Free each node in the linked list starting with sentinel->next and running back around to the sentinel
	 * If the deque is empty, then just delete the sentinel and move on
	 * Free the sentinel outside the loop
	 */
	while (head != NULL && head != sentinel) {
		temp = head;
		head = head->next;
		free(temp);
	}//end while
	free(sentinel);	
}//end deconstructor

/*
 * Function:	size
 *
 * Description:	return the size of the deque
 * Runtime:		O(1)
 */
int Deque::size() {
	assert(this != NULL);
	return count;
}//end size

/*
 * Function:	addFirst
 *
 * Description:	add this data into a new node and insert the node into the front of deque
 * Runtime:		O(1)
 */
void Deque::addFirst(int x) {
	assert(this!=NULL);
	Node *np = new Node();
	np->data = x;
	/* Insert the new node into the circular doubly linked list directly after the sentinel(sentinel->next = np).*/
	this->sentinel->next->prev = np;
	np->next = sentinel->next;
	np->prev = sentinel;
	sentinel->next = np;
	count++;
}//end addFirst

/*
 * Function:	addLast
 *
 * Description:	add x to a new node and insert the node into the back of the deque
 * Runtime:		O(1)
 */
void Deque::addLast(int x) {
	assert(this!=NULL);
	Node *np = new Node();
	np->data = x;
	/*Insert the new node after the sentinel (sentinel->prev = np)*/
	sentinel->prev->next = np;
	np->prev = sentinel->prev;
	np->next = sentinel;
	sentinel->prev = np;
	count++;
}//end addLast

/*
 * Function:	removeFirst
 *
 * Description:	return the data from the first element in the deque and delete the node
 * Runtime:		O(1)
 */
int Deque::removeFirst() {
	assert(this!=NULL);
	/*Return the data (int) from the node after sentinel (sentinel->next) and reassign pointers*/
	int data = sentinel->next->data;
	Node *np = sentinel->next; //temporary copy
	sentinel->next = np->next;
	np->next->prev = sentinel;
	free(np);
	count--;
	return data;
}//end removeFirst

/*
 * Function:	removeLast
 *
 * Description:	return the data from the last element in the deque and delete the node
 * Runtime:		O(1)
 */
int Deque::removeLast() {
	assert(this!=NULL);
	/*Return the data from the node before sentinel (sentinel->prev) and reassign pointers*/
	int data = sentinel->prev->data;
	Node *np = sentinel->prev; //temporary copy
	sentinel->prev = np->prev;
	np->prev->next = sentinel;
	free(np);
	count--;
	return data;
}//end removeLast

/*
 * Function:	getFirst
 *
 * Description:	return the data from the first node in the deque
 * Runtime:		O(1)
 */
int Deque::getFirst() {
	assert(this!=NULL);
	return sentinel->next->data;
}//end getFirst

/*
 * Function:	getLast
 *
 * Description:	return the data from the last node in the deque
 * Runtime:		O(1)
 */
int Deque::getLast() {
	assert(this!=NULL);
	return sentinel->prev->data;
}//end getLast

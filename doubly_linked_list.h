//=================================================================
// Header file for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <list>
#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

// A linked list node structure.
typedef struct llnode_t {
    void* data;
    struct llnode_t* prev;
    struct llnode_t* next;
}LLNode;

typedef struct dlinkedlist_t {
    std::list<LLNode*> nodes;
    struct llnode_t* head;
    struct llnode_t* tail;
} DLinkedList;

//===========================
/* Creating nodes and lists */
//===========================

/**
 * create_llnode
 *
 * Creates a node by allocating memory for it on the heap,
 * and initializing its previous and next pointers to NULL and its data
 * pointer to the input data pointer
 *
 * @param data A void pointer to data the user is adding to the doubly linked list.
 * @return A pointer to the linked list node
 */
LLNode* create_llnode(void* data);

/**
 * create_dlinkedlist
 *
 * Creates a doubly linked list by allocating memory for it on the heap. 
 * Initialize the size to zero, as well as head and tail pointers to NULL.
 *
 * @return A pointer to an empty dlinkedlist
 */
DLinkedList* create_dlinkedlist(void);

//============================
/* Accessing nodes and lists */
//============================

/**
 * getSize
 *
 * Return the size of the doubly linked list
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @return  the size
 */
int getSize(DLinkedList* dLinkedList);

/**
 * getHead
 *
 * Return the head of the doubly linked list
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @return A pointer to an LLNode
 */
LLNode* getHead(DLinkedList* dLinkedList);

/**
 * getTail
 *
 * Return the tail of the doubly linked list
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @return A pointer to an LLNode
 */
LLNode* getTail(DLinkedList* dLinkedList);

/**
 * getNext
 *
 * Return the next node after the input node.
 *
 * @param node A pointer to an LLNode
 * @return A pointer to an LLNode
 */
LLNode* getNext(LLNode* node);

/**
 * getPrev
 *
 * Return the previous node before the input node.
 *
 * @param node A pointer to an LLNode
 * @return A pointer to an LLNode
 */
LLNode* getPrev(LLNode* node);

/**
 * getData
 *
 * Return the input node's data.
 *
 * @param node A pointer to an LLNode
 * @return A void pointer to the node's data.
 */
void* getData(LLNode* node);

/**
 * insertHead
 *
 * Create a new LLNode with the given data and insert it at the head of the
 * doubly linked list. Update head/tail/size if necessary.
 * Note: The case where dLinkedList is not empty can be implemented by calling
 * insertBefore with the head of the list as the next_node.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param data A void pointer to data the user is adding to the doubly linked list.
 * 
 */
void insertHead(DLinkedList* dLinkedList, void* data);



/**
 * deleteNode
 *
 * Delete the node pointed to by Node (splice it out).  
 * Update head/tail/size if necessary.
 * Assumes Node is not NULL.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param Node A pointer to a linked list node.
 */

void deleteNode(DLinkedList* dLinkedList, LLNode* Node);

/**
 * destroyList
 *
 * Destroy the doubly linked list. Everything in the doubly linked list,
 * including list structure, nodes and data, are all freed from the heap.
 *
 * @param dLinkedList A pointer to the doubly linked list
 *
 */
void destroyList(DLinkedList* dLinkedList);



#endif

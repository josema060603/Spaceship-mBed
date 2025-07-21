//=================================================================
// Implementation for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <iterator>
#include <algorithm>
#include "doubly_linked_list.h"
#include <cstdint>
#include <iostream>

using namespace std;

LLNode* create_llnode(void* data) {
    LLNode* node = new LLNode();
    node->data = data;
    node->next = nullptr;
    node->prev = nullptr;
    return node;
}


DLinkedList* create_dlinkedlist() {
    DLinkedList* newList = new DLinkedList();
    newList->head = nullptr;
    newList->tail = nullptr;
    return newList;
}


int getSize(DLinkedList* dLinkedList){
  return dLinkedList->nodes.size();
}

LLNode* getHead(DLinkedList* dLinkedList){
  return dLinkedList->head;
}

LLNode* getTail(DLinkedList* dLinkedList){

  return dLinkedList->tail; // replace this
}

LLNode* getNext(LLNode* node){

  return node->next; 
}

LLNode* getPrev(LLNode* node){

  return node->prev; 

}

void* getData(LLNode* node){

  return node->data; 
}


void insertHead(DLinkedList* dLinkedList, void* data){
    LLNode* newNode = create_llnode(data);
    if (dLinkedList->head != nullptr) {
        newNode->next = dLinkedList->head;
        dLinkedList->head->prev = newNode;
    } else {
        dLinkedList->tail = newNode;
    }
    dLinkedList->head = newNode;
    dLinkedList->nodes.push_front(newNode);
  
}

void deleteNode(DLinkedList* dLinkedList, LLNode* node){

  if (dLinkedList->nodes.size() == 0)
    return; 
  
  auto it = std::find(dLinkedList->nodes.begin(), dLinkedList->nodes.end(), node);
    if (it != dLinkedList->nodes.end()) {
        if ((*it)->prev != nullptr)
            (*it)->prev->next = (*it)->next;
        else
            dLinkedList->head = (*it)->next;


        if ((*it)->next != nullptr)
            (*it)->next->prev = (*it)->prev;
        else
            dLinkedList->tail = (*it)->prev;


        dLinkedList->nodes.erase(it);
        ::operator delete(node->data);
        delete node;
    }
}

void destroyList(DLinkedList* dLinkedList){
  for (LLNode* node : dLinkedList->nodes) {
        ::operator delete(node->data);
        delete node;
    }
    dLinkedList->nodes.clear();
    delete dLinkedList;
}


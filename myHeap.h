//
//  myHeap.h
//  J_C
//
//  Created by Jyothi Jayanthi on 5/3/17.
//  Copyright © 2017 Jyothi Jayanthi. All rights reserved.
//

#ifndef myHeap_h
#define myHeap_h

#include <stdio.h>

/** @param Max values to limit in a heap. **/

#define MAX_VALUES 32

/** @para This array will store the MAX_VALUES numbers in Heap **/
extern uint16_t heap[MAX_VALUES];

/** @para This indicates the size of heap filled **/
extern uint16_t heap_size;



/** Builds heap on given array of random inputs.
 **/
void buildHeap();

/** Swap values down the Heap. **/
void heapifyDown();

/** Swap values up the Heap. **/
void heapifyup();

/** Extracts min element from the heap
 @return returns top of the heap, which is the min element, else -1 if
 heap is empty.
 **/
int peek();

/** Extracts root element from Heap.
 @returns negative number if Heap is empty.
 **/
int heap_poll();

#endif /* myHeap_h */

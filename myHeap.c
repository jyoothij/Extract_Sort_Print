//
//  myHeap.c
//  J_C
//
//  Created by Jyothi Jayanthi on 5/3/17.
//  Copyright © 2017 Jyothi Jayanthi. All rights reserved.
//

#include "myHeap.h"
#include "math.h"
#include "stdlib.h"

/** @para This array will store the MAX_VALUES numbers in Heap **/
 uint16_t heap[MAX_VALUES];

/** @para This indicates the size of heap filled **/
 uint16_t heap_size = 0;

/** @param parentIndex The index of the parent element.
    @return The index of the left child.
 **/
uint16_t getLeftChildIndex(uint16_t parentIndex)
{
    return 2 * parentIndex + 1;
}

/** @param parentIndex The index of the parent element.
    @return The index of the right child.
 **/
uint16_t getRightChildIndex(uint16_t parentIndex)
{
    return 2 * parentIndex + 2;
}

/** @param childIndex The index of the child element.
    @return The index of the parent element.
 **/
uint16_t getParentIndex(uint16_t childIndex)
{
    return (childIndex - 1) / 2;
}

/** @param index The index of the element you are checking.
    @return true if the Heap contains enough elements to fill the left child index,
            false otherwise.
**/
_Bool hasLeftChild(uint16_t index)
{
    return getLeftChildIndex(index) < heap_size;
}

/** @param index The index of the element you are checking.
    @return true if the Heap contains enough elements to fill the right child index,
            false otherwise.
**/
_Bool hasRightChild(uint16_t index)
{
    return getRightChildIndex(index) < heap_size;
}

/** @param index The index of the element you are checking.
    @return true if the calculated parent index exists within array bounds
            false otherwise.
**/
_Bool hasParent(uint16_t index)
{
    return getParentIndex(index) >= 0;
}

/** @param index The index of the element whose child you want.
    @return the value in the left child.
**/
uint16_t leftChild(uint16_t index)
{
    return heap[getLeftChildIndex(index)];
}

/** @param index The index of the element whose child you want.
    @return the value in the right child.
**/
uint16_t rightChild(uint16_t index)
{
    return heap[getRightChildIndex(index)];
}

/** @param index The index of the element you are checking.
    @return the value in the parent element.
**/
uint16_t parent(uint16_t index)
{
    return heap[getParentIndex(index)];
}

/** @param indexOne The first index for the pair of elements being swapped.
    @param indexTwo The second index for the pair of elements being swapped.
**/
void swap(uint16_t indexOne, uint16_t indexTwo)
{
    uint16_t temp = heap[indexOne];
    heap[indexOne] = heap[indexTwo];
    heap[indexTwo] = temp;
}

/** @Extracts min element from the heap
    @return returns top of the heap, which is the min element, else -1 if
            heap is empty.
 **/
int peek()
{
    
        return heap[0];
    
}


/** Swap values down the Heap. **/
void heapifyDown()
{
    uint16_t index = 0;
    while(hasLeftChild(index)) {
        uint16_t smallerChildIndex = getLeftChildIndex(index);
        
        if( hasRightChild(index)
           && rightChild(index) < leftChild(index)
           ) {
            smallerChildIndex = getRightChildIndex(index);
        }
        
        if(heap[index] < heap[smallerChildIndex]) {
            break;
        }
        else {
            swap(index, smallerChildIndex);
        }
        index = smallerChildIndex;
    }
}

/** Swap values up the Heap. **/
void heapifyUp()
{
    uint16_t index = heap_size - 1;
    while(    hasParent(index)
          &&   parent(index) > heap[index]
          )
    {
        swap(getParentIndex(index), index);
        index = getParentIndex(index);
    }
}


/** Extracts root element from Heap.
    @returns negative number if Heap is empty.
 **/
int heap_poll()
{
    if(heap_size == 0)
    {
        //nothing to poll
        return -1;
    }
    else
    {
        int item = heap[0];
        heap[0] = heap[heap_size - 1];
        heap_size--;
        heapifyDown();
        return item;
    }
}

/** Heapify the given input, the top of the heap will always have
    min lelement/
**/
void MinHeapify(int i)
{
    uint16_t l = getLeftChildIndex(i);
    uint16_t r = getRightChildIndex(i);
    uint16_t smallest = i;
    if (l < heap_size && heap[l] < heap[i])
        smallest = l;
    if (r < heap_size && heap[r] < heap[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(i,smallest);
        MinHeapify(smallest);
    }
}

/** Builds heap on given array of random inputs.
 **/
void buildHeap()
{
    int i = (heap_size - 1)/2;
    while (i >= 0)
    {
        MinHeapify(i);
        i--;
    }
}




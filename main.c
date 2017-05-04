//
//  main.c
//  J_C
//
//  Created by Jyothi Jayanthi on 12/15/16.
//  Copyright © 2016 Jyothi Jayanthi. All rights reserved.
//


#include "stdio.h"
#include "stdlib.h"
#include "myHeap.h"

#define NIBBLE 4
#define NUM_BITS_TO_READ 12
#define BYTE 8
#define LOW_NIBBLE_MASK 0x0F

// this circular buffer of the last 32 values read
uint16_t read_buff[MAX_VALUES];

//circular buffer index
uint16_t read_buff_idx = 0;

/** FUNCTIONALITY:
    This function is used in comparision function of quick sort.
    @param a b pointers needs to be comared.
 **/
int cmpfunc (const void * a, const void * b)
{
    return ( *(uint16_t*)a - *(uint16_t*)b );
}

/** FUNCTIONALITY:
    This function adds the extracted item into the min heap
    @param item to be inserted into the min Heap.
 **/
void addToHeap(uint16_t item)
{
    //  For the first MAX SIZE elements, insert directly.
    //  When the limit goes beyond MAX VALUES, build the
    //  heap and start adding only if the incoming number
    //  is greater than the minimum element.
    static uint16_t count = 0;
    if(count < MAX_VALUES)
    {
        heap[count++] = item;
        heap_size = count;
    }
    else
    {
        if(count == MAX_VALUES)
        {
            buildHeap();
        }
        
        int min_element = peek();
        if(item >= min_element)
        {
            heap[0] = item;
            heapifyDown();
        }
        count++;
    }
}



/**  FUNCTIONALITY:
        This function ints the read buffer.
 **/
void init_log_array()
{
    static _Bool is_init = 0;
    if(!is_init)
    {
        for(int i =0;i<MAX_VALUES;i++)
        {
            read_buff[i] = 0;
        }
        is_init = 1;
    }
    else
    {
        return;
    }
}


/** FUNCTIONALITY:
        This function buffers the last read values into a circular buffer of
        size 32 entries.
        @param num to log in read_buffer
**/
void log_numbers(uint16_t num)
{
    init_log_array();
    
    if((read_buff_idx) == MAX_VALUES)
    {
        read_buff_idx = 0;
    }
    read_buff[read_buff_idx++] = num;
}

/** FUNCTIONALITY:
        This function writes to the output file
        @param w_ptr to the file where values needs to be written
**/
void writeTofile(FILE* w_ptr)
{
    const char *sort_text = "--Sorted Max 32 Values--";
    fprintf(w_ptr, "%s\n", sort_text);
    
    // if heap_size = 0 and read_buffer_size = 0, then nothing is read,
    // so do nothing
    
    // else, post process heap and read_buffer
   
    if( (heap_size != 0) && (heap_size < MAX_VALUES) )
    {
        //have not sorted the array, build heap now
        buildHeap();
    }
   
    while(heap_size > 0)
    {
        int min_element = heap_poll();
        printf("min element is %d \n",min_element);
        
        fprintf(w_ptr,"%d \n", min_element);
    }
    
    const char* log_text = "--Last 32 Values--";
    fprintf(w_ptr, "%s\n", log_text);
    
    uint16_t max_values_to_print = MAX_VALUES;
    if( (read_buff_idx !=0) &&(read_buff_idx < MAX_VALUES) )
    {
        max_values_to_print = read_buff_idx;
        read_buff_idx = 0;     
    }
    for(int i = read_buff_idx; ; )
    {
        fprintf(w_ptr,"%d \n",read_buff[i]);
        i = (i+1)%max_values_to_print;
        if(i == read_buff_idx)
            break;
    }
}


/** FUNCTIONALITY :
        This function extracts 6 bytes in a go, which is 6*8 = 48 bits values
        from the binary stream in the file.
        When we read 48 bits, that will have four 12 bit values - 48/12 = 4
        This approach reduces the load of byte by byte reading.
        @param ip_file to read the values and op_file to write the sorted/latest 
        values.
 **/
int extract_and_store(const char* ip_file , const char* op_file)
{
    FILE *ptr_read_myfile = NULL , *ptr_write_myfile = NULL;
    uint8_t my_number[6] = {0};
    uint16_t extracted_number = 0;
    unsigned long loop_count = 0;
    unsigned long res = 1 ;
    
    
    if( (ip_file == NULL) || (op_file == NULL) )
    {
        printf("Error.. check input file / output file passed.. \n");
        return 1;
    }
    
    ptr_read_myfile=fopen(ip_file,"rb");
    
    if (!ptr_read_myfile)
    {
        printf("Error .. Unable to open file to read! \n");
        return 1;
    }
    
    ptr_write_myfile=fopen(op_file,"w");
    
    if (!ptr_write_myfile)
    {
        printf("Error .. Unable to open file to write! \n");
        return 1;
    }
    
    /*  -   Basically, we can either read input file in bigger chunks multiple times
     or have all the file read in one go into a dynamically allocated memory
     and post process it.
     -   Reading the file multiple times will sure take a performace hit, as the
     file is stored in slow memory, but reading it in bigger chunks can
     optimize this read delay.
     -   Reading all the file in one go and then storing it in a dynamically
     allocated buffer is another option. But if we are a stictly memory
     limited system, dynamic allocation of such a bug chunk can cause heap
     exhaustion for other modules.
     -   My solution here proesents readings chunks of 48 bits in one go which
     I think might be good trade off between multiple file reading attemps
     and memory.
     */
    while ( (!feof(ptr_read_myfile) ) )
    {
        res = fread(&my_number,sizeof(uint8_t),6,ptr_read_myfile);
        
        res = (res*BYTE)/NUM_BITS_TO_READ;
        
        if(0 == res)
        {
            // less than 12 bites, ignore.
            break;
        }
        else
        {
            loop_count = res;
        }
        
        /* basically tried to implement this logic in a generic way
         first_12_bit_number = (my_number[0] << 4) | (my_number[1] >> 4);
         second_12_bit_number = ((my_number[1] & 0x0F) << 8)| (my_number[2]  );
         third_12_bit_number = (my_number[3] <<4) | (my_number[4] >> 4);
         forth_12_bit_number = ( (my_number[4] & 0x0F) <<8)| (my_number[5]);
         */
        
        for(int i = 0 , j = 0; i< loop_count;i++,j++)
        {
            uint16_t num1 = my_number[j];
            uint16_t num2 = my_number[j+1];
            if(i%2 == 0)
            {
                extracted_number = (num1 << NIBBLE) | (num2 >> NIBBLE);
            }
            else
            {
                extracted_number = (( num1 & LOW_NIBBLE_MASK) << BYTE)| (num2);
                j++;
            }
            addToHeap(extracted_number);
            log_numbers(extracted_number);
        }
    }
    
    fclose(ptr_read_myfile);
    
    /* Now write to the file */
    writeTofile(ptr_write_myfile);
    fclose(ptr_write_myfile);
    
    return 0;
}

// Driver code;
int main()
{
    // extract the 12 bit values
    
    /*  STUB - not sure how the file names are passed here, so stubbing this part
     get file names into const char* input_file_name , const char* output_file_name
     */
    /* Example:
    const char* input_file_name = "/Users/Jyothi/Desktop/inputs/test3.bin";
    const char* output_file_name = "/Users/Jyothi/Desktop/inputs/Jotest3.txt";
    */
    
    if(extract_and_store(input_file_name , output_file_name))
        printf("Error.. could not extract the values \n");
    else
        printf("Done extracting the values \n");
    
    return 0;
}

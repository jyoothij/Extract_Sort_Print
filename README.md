# Extract_Sort_Print
This program Extracts 12 bit values , sort them and print to a file

README

This is a program for implementing a 
1. 12 bit read from stream of inputs
2. Sorting the inputs with Min-Heap or Priority Queue in plain C. 
3. Printing the highest 32 values to the file.



- Min heap implementation makes use of a plain array to store the table. It does not supports resizing (growing and shrinking).
- Min heap can be used as a library.
- To use the library you only need to include the heap.h header, and link against the heap.c file. There is nothing fancy required.
- Min heap ensures random inputs are taken care of.

Testing the solution
Included in the project is a file main.c which serves as a simple test file.To test this, just run make and then run the test program.

note: make commands are in text file, make a local make file and copy these in there and run it from command prompt.

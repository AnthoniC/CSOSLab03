# Lab 3: The Heap
### 100 Points
### Name: Anthoni Caravello

**Objective:** Create a heap

This is the next a multi-lab project to create aspects of your own operating system entirely within userspace. We will continue in our ability to manage memory. In this lab we will create a heap space. For these labs, you should modify **this readme** file so I have an in project identifier. 

Your stack needs the capacity to:
+ Allocate data of arbitrary size
+ Retain the size of the data allocated
+ You may chose to implement data via linked list free list or in chunks of predesigned size (similar to how Apple allocates small chunks of data). I chose linked list in my implementation, but you may chose something else if you prefer. If you want an approach that is neither of these *you must receive approval in advance* 
+ Implement all of the methods defined in `ACMemory.c` You may import your old work into this project. Note that there have been small changes to `struct OS_Memory` and `initialize_memory` in addition to new files that have been created. 
+ Correctly run all tests in `MemoryTests.h`


You may add additional structs, methods, and macros in your `ACMemory.c` file if necessary; *however*, you **CANNOT** modify `struct OS_Memory` or add additional global variables to retain data. If you have any questions, please contact me.


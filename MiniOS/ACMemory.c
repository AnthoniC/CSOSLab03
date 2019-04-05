//
//  Memory420.c
//  Lab02
//
//  Created by Aaron David Block on 1/28/19.
//  Copyright © 2019 Aaron David Block. All rights reserved.
//

#include "ACMemory.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

const size_t sizet_size = sizeof(size_t);
const size_t overhead_size = sizeof(void*) + sizet_size;

#define element_size_heap(X) *(size_t*)(X - sizet_size)
#define next_element_heap(X) *(void**)(X - overhead_size)

////Don't touch this
//typedef struct OS_Memory {
//    void* data;
//    size_t size_of_data;
//
//    size_t size_of_stack;
//    void* stack_pointer;
//    void* start_of_stack;
//
//    size_t size_of_heap;
//    void* start_of_heap;
//    void* start_of_free_list;
//} OS_Memory;


/* Initialize a memory object passed to the structure, retuns 0 if success 1 if failure.
 * *Note*: All data must be the multiple of overhead_size (16 bytes on 64bit systems). If it is less, than it will be rounded up */
int initialize_memory(OS_Memory* memory, size_t stack_size, size_t heap_size){
    // Checks to see if size is a multiple of overhead_size
    // If it is then temp is set to size, if it isn't size is rounded up and assigned to temp
    size_t temp_stack_size;
    if (stack_size%overhead_size != 0) {
        temp_stack_size = ((stack_size/overhead_size) + 1)*overhead_size;
    }
    else{
        temp_stack_size = stack_size;
    }
    if(temp_stack_size == 0){
        temp_stack_size = 16;
    }
    
    size_t temp_heap_size;
    if (heap_size%overhead_size != 0) {
        temp_heap_size = ((heap_size/overhead_size) + 1)*overhead_size;
    }
    else{
        temp_heap_size = heap_size;
    }
    if(temp_heap_size == 0){
        temp_heap_size = 16;
    }
    size_t temp_size = temp_heap_size + temp_stack_size;
    
    // Allocate memory for data and set memory's variables
    void* bag_of_data = (void*)malloc(temp_size);
    memory->data = bag_of_data;
    memory->size_of_data = temp_size;
    
    memory->size_of_stack = temp_stack_size;
    memory->start_of_stack = bag_of_data;
    memory->stack_pointer = bag_of_data;
    
    bag_of_data += temp_stack_size;
    memory->size_of_heap = temp_heap_size;
    memory->start_of_heap = bag_of_data;
    memory->start_of_free_list = bag_of_data;
    
    add_heap_size(memory->start_of_heap, memory->size_of_heap);
    add_heap_pointer(memory->start_of_heap, &memory->start_of_heap);
    
    // Check to see if memort->stack_pointer exists
    if (memory->stack_pointer != NULL) {
        return 0;
    }
    else{
        return 1;
    }
}


/* frees the memory allocated */
void free_memory(OS_Memory* memory){
    free(memory->data);
}


/* Pushes the data in bytes onto the stack and returns a pointer to the data's location
 * returns 0 if there is a failure.
 */
void* push_bytes(OS_Memory* memory, void* data, size_t size){
    // Checks to see if size is a multiple of overhead_size
    // If it is then temp is set to size, if it isn't size is rounded up and assigned to temp
    size_t temp;
    if (size%overhead_size != 0) {
        temp = ((size/overhead_size) + 1)*overhead_size;
    }
    else{
        temp = size;
    }
    
    // Check to see if there is enough room for data, if not return 0
    if((temp + overhead_size) > memory->size_of_stack){
        return 0;
    } else{
        // If there is enough room change the size_of_stack to match
        memory->size_of_stack -= (temp + overhead_size);
    }
    
    // Add the overhead information for the new data
    if(memory->stack_pointer == memory->start_of_stack){
        add_stack_element_overhead(memory->stack_pointer, temp, &memory->start_of_stack);
        memory->stack_pointer += overhead_size;
    }
    else{
        void* temp_ptr = memory->stack_pointer;
        temp_ptr -= overhead_size;
        size_t* last_element_size = (size_t*)temp_ptr;
        temp_ptr += overhead_size;
        add_stack_element_overhead((temp_ptr + *last_element_size), temp, &temp_ptr);
        temp_ptr += *last_element_size;
        temp_ptr += overhead_size;
        memcpy(temp_ptr, data, size);
        memory->stack_pointer = temp_ptr;
    }
    // Copy the data from data into the stack
    memcpy(memory->stack_pointer, data, size);
    
    return memory->stack_pointer;
}



/* Returns a pointer to the top element on the stack, 0 if there is a failure */
void* get_bytes(OS_Memory* memory){
    if(memory->stack_pointer == memory->start_of_stack){
        return 0;
    }
    return memory->stack_pointer;
}


/* Pops off top value from stack.
 * Returns pointer to the popped element, 0 if there is an error.
 * While Pointer is *removed* from the stack, it still lives in memory for a short time. Therefore, if you want to use this data you should immediatly copy it to a more persistant data location since the next push will destroy the data.
 */
void* pop_bytes(OS_Memory* memory){
    // Check to see if the list is empty, if it is return 0
    if(memory->stack_pointer == memory->start_of_stack){
        return 0;
    }
    
    // set return value
    void* to_return = memory->stack_pointer;
    
    // Set memory->stack_pointer to the previous data point on the stack
    void** last_ptr = (void**)malloc(sizeof(void*));
    void* temp = memory->stack_pointer;
    temp -= sizeof(void*);
    memcpy(last_ptr, temp, sizeof(void*));
    memory->stack_pointer = *last_ptr;
    
    // Add the size back into memory->size_of_stack
    temp -= sizet_size;
    int* size = (int*)temp;
    memory->size_of_stack += (*size + overhead_size);
    
    return to_return;
}

/* Adds the overhead information to the list
 * stack_ptr is the location to put the information
 * size is the size of the new data
 * ptr is the pointer to the last element of the stack
 */
void add_stack_element_overhead(void* stack_ptr, size_t size, void** ptr){
    
    size_t* temp_int = (size_t*)stack_ptr;
    *temp_int = size;
    void* temp_ptr = (void*)(stack_ptr + sizet_size);
    memcpy(temp_ptr, ptr, sizeof(void*));
    
    void** thingy = (void**)malloc(sizeof(void*));
    memcpy(thingy, temp_ptr, sizeof(void*));
}

/* Similar to malloc(), but all in user space */
void* memory_alloc(OS_Memory* memory, size_t size){
    size_t temp_size;
    if (size%overhead_size != 0) {
        temp_size = ((size/overhead_size) + 1)*overhead_size;
    }
    else{
        temp_size = size;
    }
    size_t over_size = temp_size + overhead_size;
    
    size_t current_spot_size = get_heap_size(memory->start_of_free_list);
    void* current_spot_ptr = memory->start_of_free_list;
    void* nxt_ptr = get_heap_pointer(memory->start_of_free_list);
    
    while(over_size > current_spot_size){
        if(nxt_ptr == memory->start_of_heap){
            return 0;
        }
        current_spot_ptr = nxt_ptr;
        current_spot_size = get_heap_size(nxt_ptr);
        nxt_ptr = get_heap_pointer(nxt_ptr);
    }
    current_spot_size -= over_size;
    
    void* next_spot_pointer;
    if(current_spot_size > 0){
        next_spot_pointer = (current_spot_ptr + over_size);
        add_heap_size(next_spot_pointer, current_spot_size);
    } else{
        next_spot_pointer = get_heap_pointer(current_spot_ptr);
        memory->start_of_free_list = next_spot_pointer;
    }
    
    add_heap_size(current_spot_ptr, temp_size);
    size_t zero = 0;
    add_heap_pointer(current_spot_ptr, &zero);
    add_heap_pointer(next_spot_pointer, &nxt_ptr);
    if(current_spot_ptr == memory->start_of_free_list){
        memory->start_of_free_list = next_spot_pointer;
    }
    
    return (current_spot_ptr + overhead_size);
}


/* Similar to free(), but all in user space */
void memory_dealloc(OS_Memory* memory, void* ptr){
    if(get_heap_size(memory->start_of_free_list) == memory->size_of_heap){
        return;
    }
    
    void* current_node_ptr = (ptr - overhead_size);
    size_t current_node_size = get_heap_size(current_node_ptr);
    void* next_node_ptr = (ptr + current_node_size);
    void* next_next_node_ptr = get_heap_pointer(next_node_ptr);
    current_node_size += overhead_size;
    add_heap_size(current_node_ptr, current_node_size);
    
    if(next_next_node_ptr){
        current_node_size += get_heap_size(next_node_ptr);
        add_heap_size(current_node_ptr, current_node_size);
        add_heap_pointer(current_node_ptr, &next_next_node_ptr);
    }
    
    
    
    if(current_node_ptr > memory->start_of_free_list){
        void* temp = memory->start_of_free_list;
        void* next = get_heap_pointer(temp);
        while(next && next != memory->start_of_heap && next < current_node_ptr){
            temp = next;
            next = get_heap_pointer(next);
        }
        if(temp + get_heap_size(temp) == current_node_ptr){
            current_node_size += get_heap_size(temp);
            add_heap_size(temp, current_node_size);
            void* out_of_names = get_heap_pointer(current_node_ptr);
            add_heap_pointer(temp, &out_of_names);
        } else{
            add_heap_pointer(temp, &current_node_ptr);
            add_heap_pointer(current_node_ptr, &next);
        }
    } else{
        void* SOFL = memory->start_of_free_list;
        add_heap_pointer(current_node_ptr, &SOFL);
        memory->start_of_free_list = current_node_ptr;
    }
    
    
    
    if(current_node_ptr < memory->start_of_free_list){
        
    }
}


void add_heap_pointer(void* start_of_element, void* ptr_to_add){
    void* temp_ptr = (void*)(start_of_element + sizet_size);
    memcpy(temp_ptr, ptr_to_add, sizeof(void*));
}

void add_heap_size(void* start_of_element, size_t size_to_add){
    size_t* temp_size = (size_t*)start_of_element;
    *temp_size = size_to_add;
}

void* get_heap_pointer(void* start_of_element){
    void* temp = start_of_element;
    temp += sizet_size;
    void** temp_ptr = (void**)malloc(sizeof(void*));
    memcpy(temp_ptr, temp, sizeof(void*));
    return *temp_ptr;
}

size_t get_heap_size(void* start_of_element){
    void* temp = start_of_element;
    size_t* temp_int = (size_t*)temp;
    
    return *temp_int;
}


int is_free(void* ptr){
    
    
    return 0;
}

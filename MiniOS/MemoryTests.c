//
//  MemoryTests.c
//  Lab02
//
//  Created by Aaron David Block on 1/29/19.
//  Copyright © 2019 Aaron David Block. All rights reserved.
//

#include "MemoryTests.h"
#include "ACMemory.h"
#include <assert.h>
#include <stdio.h>


// Code to include Macro for printing debug code. So, we can unclutter our test.
#ifndef DEBUGGER_PRINTER
#define DEBUGGER_PRINTER

//DEBUG_LEVEL 0 means no comments
//DEBUG_LEVEL 1 means no Start and stop defined
//DEBUG_LEVEL 2 means All comments are on
#define DEBUG_LEVEL 1


#if DEBUG_LEVEL <= 0
    #define DEBUG_PRINT(fmt, args...)
    #define TEST_NAME
    #define TEST_PASSED
#else
    // __func__ is part of C that gets the method name that we're currently in. 
    #define TEST_START printf("\\/ \\/ \\/ \\/ \t Running %s \t\\/ \\/ \\/ \\/\n", __func__);
    #define TEST_PASSED printf("/\\ /\\ /\\ /\\ \t %s passed! \t/\\ /\\ /\\ /\\\n\n", __func__);
    #if DEBUG_LEVEL >= 2
        #define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ##args)
    #else
        #define DEBUG_PRINT(fmt, args...)
    #endif
#endif

#endif


enum Month {
    Jan, Feb, March, April, May, June, July, Aug, Sep, Nov, Dec
};

struct date {
    int year;
    enum Month month;
    int day;
};

int date_eq(struct date d1, struct date d2){
    if (d1.year==d2.year && d1.month==d2.month && d1.year == d2.year){
        return 1;
    } else {
        return 0;
    }
}

void basic_heap_test(){
    TEST_START
    
    OS_Memory memory;
    
    initialize_memory(&memory, 1024,1024);
    
    char* a_ptr = memory_alloc(&memory, sizeof(char));
    char* b_ptr = memory_alloc(&memory, sizeof(char));
    char* c_ptr = memory_alloc(&memory, sizeof(char));
    assert(a_ptr && b_ptr && c_ptr && "Pointer not allocated correctly\n");
    
    *a_ptr = 'a';
    *b_ptr = 'b';
    *c_ptr = 'c';
    DEBUG_PRINT("a:%c, b:%c, c:%c\n", *a_ptr, *b_ptr, *c_ptr);
    DEBUG_PRINT("Locations:\n");
    DEBUG_PRINT("\ta:%p\n\tb:%p\n\tc:%p\n", a_ptr, b_ptr, c_ptr);
    
    memory_dealloc(&memory, a_ptr);
    char* d_ptr = memory_alloc(&memory, sizeof(char));
    char* e_ptr = memory_alloc(&memory, sizeof(char));
    assert(d_ptr && b_ptr && c_ptr && e_ptr && "Pointer not allocated correctly\n");
    
    *d_ptr = 'd';
    *e_ptr = 'e';
    //By MY implementation d_ptr should have replaced a_ptr, yours may be different
    DEBUG_PRINT("d:%c, b:%c, c:%c, e:%c\n", *d_ptr, *b_ptr, *c_ptr, *e_ptr);
    DEBUG_PRINT("Locations:\n");
    DEBUG_PRINT("\td:%p\n\tb:%p\n\tc:%p\n\te:%p\n", d_ptr, b_ptr, c_ptr, e_ptr);
    assert(*d_ptr == 'd' && *b_ptr == 'b' && *c_ptr == 'c' && *e_ptr == 'e' && "Pointer not storing correctly\n");
    
    memory_dealloc(&memory, b_ptr);
    memory_dealloc(&memory, e_ptr);
    memory_dealloc(&memory, c_ptr);
    memory_dealloc(&memory, d_ptr);
    
    //Does this crash?
    memory_dealloc(&memory, d_ptr);
    char* w_ptr = memory_alloc(&memory, sizeof(char));
    char* z_ptr = memory_alloc(&memory, sizeof(char));
    assert(w_ptr && z_ptr && "Pointer not allocated correctly after BAD free\n");
    
    *w_ptr = 'w';
    *z_ptr = 'z';
    assert(*w_ptr == 'w' && *z_ptr == 'z'&& "Pointer not storing correctly after BAD free\n");
    DEBUG_PRINT("w:%c, z:%c\n", *w_ptr, *z_ptr);
    DEBUG_PRINT("Locations:\n");
    DEBUG_PRINT("\tw:%p\n\tz:%p\n", w_ptr, z_ptr);
    
    
    free_memory(&memory);
    TEST_PASSED
}


void multisize_heap_test(){
    TEST_START
    OS_Memory memory;
    
    initialize_memory(&memory, 4096,4096);
    
    int* a_ptr = memory_alloc(&memory, sizeof(int));
    char* b_ptr = memory_alloc(&memory, sizeof(char));
    double* c_ptr = memory_alloc(&memory, sizeof(double));
    int* d_ptr = memory_alloc(&memory, sizeof(int));
    assert(a_ptr && b_ptr && c_ptr &&  d_ptr && "Pointer not allocated correctly\n");
    
    *a_ptr = 420;
    *b_ptr = 'b';
    double c = -23.2;
    *c_ptr = c;
    *d_ptr = 66;
    assert(*a_ptr == 420 && *b_ptr == 'b' && *c_ptr == c && *d_ptr==66 && "Pointer not asigning correctly\n");
    
    
    DEBUG_PRINT("a:%d, b:%c, c:%f, d:%d\n", *a_ptr, *b_ptr, *c_ptr, *d_ptr);
    DEBUG_PRINT("Locations:\n");
    DEBUG_PRINT("\ta:%p\n\tb:%p\n\tc:%p\n\td:%p\n", a_ptr, b_ptr, c_ptr, d_ptr);
    
    memory_dealloc(&memory, b_ptr);
    double* e_ptr = memory_alloc(&memory, sizeof(double));
    int* f_ptr = memory_alloc(&memory, sizeof(int));
    char* g_ptr = memory_alloc(&memory, sizeof(char));
    assert(e_ptr && f_ptr && g_ptr && "Pointer not allocated correctly\n");
    
    
    double e = 65.44;
    *e_ptr = e;
    *f_ptr = 555;
    *g_ptr = 'g';
    
    DEBUG_PRINT("e:%f, f:%d, g:%c\n", *e_ptr, *f_ptr, *g_ptr);
    DEBUG_PRINT("Locations:\n");
    DEBUG_PRINT("\te:%p\n\tf:%p\n\tg:%p\n", e_ptr, f_ptr, g_ptr);
    
    assert(*e_ptr == e && *f_ptr == 555 && *g_ptr == 'g' && "Pointer not asigning correctly\n");
    
    
    free_memory(&memory);
    TEST_PASSED
}


void array_heap_test(){
    TEST_START
    OS_Memory memory;
    
    initialize_memory(&memory, 4096,4096);
    int a[5] = {10,20,30,40,50};
    char b = 'b';
    int c[4] = {60,70,80,90};
    int d[3] = {100,110,120};
    int e[3] = {120,130,140};
    int g[3] = {520,160,740};
    int h[5] = {999,888,123, 556, 765};

    int* a_ptr = memory_alloc(&memory, sizeof(int)*5);
    char* b_ptr = memory_alloc(&memory, sizeof(char));
    int* c_ptr = memory_alloc(&memory, sizeof(int)*4);
    int* d_ptr = memory_alloc(&memory, sizeof(int)*3);
    int* e_ptr = memory_alloc(&memory, sizeof(int)*3);
    assert(a_ptr && b_ptr && c_ptr && d_ptr && e_ptr && "Pointer not allocated correctly\n");
    
    for(int i =0;i< 5;i++){
        a_ptr[i] = a[i];
    }
    *b_ptr = b;
    for(int i =0;i< 4;i++){
        c_ptr[i] = c[i];
    }
    for(int i =0;i< 3;i++){
        d_ptr[i] = d[i];
    }
    
    for(int i =0;i< 3;i++){
        e_ptr[i] = e[i];
    }
    
    //test assigningmes went well
    for(int i =0;i< 5;i++){
        assert(a_ptr[i] == a[i]);
    }
    assert(*b_ptr == b);
    for(int i =0;i< 4;i++){
        assert(c_ptr[i] == c[i]);
    }
    for(int i =0;i< 3;i++){
        assert(d_ptr[i] == d[i]);
    }
    
    for(int i =0;i< 3;i++){
        assert(e_ptr[i] == e[i]);
    }
    
    //One removal test
    memory_dealloc(&memory, b_ptr);
    
    char f = 'f';
    char* f_ptr = memory_alloc(&memory, sizeof(char));
    int* g_ptr = memory_alloc(&memory, sizeof(int)*3);
    
    assert(a_ptr &&  c_ptr && d_ptr && e_ptr && f_ptr && g_ptr && "Pointer not allocated correctly\n");
    
    *f_ptr = f;
    for(int i =0;i< 3;i++){
        g_ptr[i] = g[i];
    }
    
    for(int i =0;i< 5;i++){
        assert(a_ptr[i] == a[i]);
    }
    for(int i =0;i< 4;i++){
        assert(c_ptr[i] == c[i]);
    }
    for(int i =0;i< 3;i++){
        assert(d_ptr[i] == d[i]);
    }
    
    for(int i =0;i< 3;i++){
        assert(e_ptr[i] == e[i]);
    }
    
    assert(*f_ptr == f);
    for(int i =0;i< 3;i++){
        assert(g_ptr[i] == g[i]);
    }
    
    //two removal test
    DEBUG_PRINT("Location of c is%p\n", c_ptr);
    DEBUG_PRINT("Location of d is%p\n", d_ptr);
    
    memory_dealloc(&memory, d_ptr);
    memory_dealloc(&memory, c_ptr);
    
    int* h_ptr = memory_alloc(&memory, sizeof(int)*5);
    assert(a_ptr && e_ptr && f_ptr && g_ptr && h_ptr && "Pointer not allocated correctly\n");
    
    DEBUG_PRINT("Location of h is%p\n", h_ptr);
    
    for(int i =0;i< 5;i++){
        h_ptr[i] = h[i];
    }
    
    for(int i =0;i< 5;i++){
        assert(a_ptr[i] == a[i]);
    }
    
    for(int i =0;i< 3;i++){
        assert(e_ptr[i] == e[i]);
    }
    
    assert(*f_ptr == f);
    for(int i =0;i< 3;i++){
        assert(g_ptr[i] == g[i]);
    }
    
    for(int i =0;i< 5;i++){
        assert(h_ptr[i] == h[i]);
    }

    free_memory(&memory);
    TEST_PASSED
}


void basic_stack_test(){
    TEST_START
    OS_Memory memory;
    
    initialize_memory(&memory, 100,0);
    char a = 'a';
    char b = 'b';
    char c = 'c';
    char d = 'd';
    
    char* rtnData;
    
    push_bytes(&memory, &a, sizeof(char));
    rtnData = get_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'a'&& "Cannot push data\n");
    
    push_bytes(&memory,&b, sizeof(char));
    rtnData = get_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'b'&& "Cannot push data\n");
    
    push_bytes(&memory,&c, sizeof(char));
    rtnData = get_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'c'&& "Cannot push data\n");
    
    rtnData = (char*)pop_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'c' && "Cannot pop data\n");
    
    rtnData = (char*)get_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'b' && "Cannot pop data\n");
    
    push_bytes(&memory,&d, sizeof(char));
    rtnData = get_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData== 'd' && "Cannot push after pop\n");
    
    rtnData = pop_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'd'  && "Cannot pop\n");
    
    rtnData = get_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'b'  && "Cannot get after pop\n");
    
    rtnData = pop_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'b'  && "Cannot multiple pop\n");
    
    rtnData = get_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'a'  && "Cannot multiple pop\n");
    
    rtnData = pop_bytes(&memory);
    DEBUG_PRINT("The value is %c\n", *rtnData);
    assert(*rtnData == 'a' && "Cannot pop to bottom\n");
    
    rtnData = get_bytes(&memory);
    DEBUG_PRINT("Pointer value is %p\n", rtnData);
    assert(rtnData == 0  && "Cannot get when empty\n");
    
    rtnData = pop_bytes(&memory);
    DEBUG_PRINT("Pointer value is %p\n", rtnData);
    assert(rtnData == 0  && "Cannot pop when empty\n");
    
    
    free_memory(&memory);
    TEST_PASSED
    
}

void basic_int_test(){
    TEST_START
    OS_Memory memory;
    
    initialize_memory(&memory, 100,0);
    int a = 11;
    int b = 22;
    int c = 33;
    int rtnInt = -1;
    int* rtnData;
    
    push_bytes(&memory, &a, sizeof(int));
    rtnData = get_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == a  && "Cannot push data\n");
    
    push_bytes(&memory,&b, sizeof(int));
    rtnData = get_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == b  && "Cannot push data\n");
    
    push_bytes(&memory,&c, sizeof(int));
    rtnData = get_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == c  && "Cannot push data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == c  && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == b  && "Cannot pop data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == b  && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == a  && "Cannot pop data\n");
    
    free_memory(&memory);
    TEST_PASSED
}

void basic_double_test(){
    TEST_START
    OS_Memory memory;
    
    initialize_memory(&memory, 100,0);
    double a = 2.0;
    double b = 12.4;
    double c = -35.2;
    double rtn_double = -1;
    double* rtnData;
    
    

    push_bytes(&memory, &a, sizeof(double));
    rtnData = get_bytes(&memory);
    rtn_double = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtn_double);
    
    //Normally, we shouldn't compare doubles for equality; however, since we are storing the same bits
    //this should be fine.
    assert(rtn_double == a  && "Cannot push data\n");
    
    
    push_bytes(&memory,&b, sizeof(double));
    rtnData = get_bytes(&memory);
    rtn_double = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtn_double);
    assert(rtn_double == b  && "Cannot push data\n");
    
    
    push_bytes(&memory,&c, sizeof(double));
    rtnData = get_bytes(&memory);
    rtn_double = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtn_double);
    assert(rtn_double == c  && "Cannot push data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtn_double = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtn_double);
    assert(rtn_double == c  && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtn_double = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtn_double);
    assert(rtn_double == b  && "Cannot pop data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtn_double = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtn_double);
    assert(rtn_double == b  && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtn_double = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtn_double);
    assert(rtn_double == a  && "Cannot pop data\n");
    
    free_memory(&memory);
    TEST_PASSED
}

void basic_mixed_test(){
    TEST_START
    OS_Memory memory;
    void* rtnData;
    
    initialize_memory(&memory, 100,0);
    int a = 11;
    char b = 'd';
    double c = -15.5;
    int rtnInt = 0;
    char rtnChar = 0;
    double rtnDouble = 0;
    
    

    push_bytes(&memory,&a, sizeof(int));
    rtnData = get_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == a  && "Cannot push data\n");
    
    

    push_bytes(&memory,&b, sizeof(char));
    rtnData = get_bytes(&memory);
    rtnChar = *(char*)rtnData;
    DEBUG_PRINT("The value is %c \n", rtnChar);
    assert(rtnChar == b  && "Cannot push data\n");
    
    

    push_bytes(&memory,&c, sizeof(double));
    rtnData = get_bytes(&memory);
    rtnDouble = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtnDouble);
    assert(rtnDouble == c  && "Cannot push data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtnDouble = *(double*)rtnData;
    DEBUG_PRINT("The value is %f \n", rtnDouble);
    assert(rtnDouble == c  && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtnChar = *(char*)rtnData;
    DEBUG_PRINT("The value is %c \n", rtnChar);
    assert(rtnChar == b  && "Cannot pop data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtnChar = *(char*)rtnData;
    DEBUG_PRINT("The value is %c \n", rtnChar);
    assert(rtnChar == b  && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtnInt = *(int*)rtnData;
    DEBUG_PRINT("The value is %d \n", rtnInt);
    assert(rtnInt == a  && "Cannot pop data\n");
    
    free_memory(&memory);
    TEST_PASSED
}

void basic_struct_test(){
    TEST_START
    OS_Memory memory;
    void* rtnData;
    
    initialize_memory(&memory, 100,0);
    struct date a;
    a.day = 20;
    a.month = April;
    a.year = 2019;
    
    struct date b;
    b.day = 1;
    b.month = Jan;
    b.year = 2;
    
    struct date c;
    c.day = 31;
    c.month = Dec;
    c.year = 1980;
    
    struct date rtdStruct;
    
    push_bytes(&memory,&a, sizeof(struct date));
    rtnData = get_bytes(&memory);
    rtdStruct = *(struct date*)rtnData;
    assert(date_eq(rtdStruct, a) && "Cannot push data\n");
    
    
    push_bytes(&memory,&b, sizeof(struct date));
    rtnData = get_bytes(&memory);
    rtdStruct = *(struct date*)rtnData;
    assert(date_eq(rtdStruct, b) && "Cannot push data\n");
    

    push_bytes(&memory,&c, sizeof(struct date));
    rtnData = get_bytes(&memory);
    rtdStruct = *(struct date*)rtnData;
    assert(date_eq(rtdStruct, c) && "Cannot push data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtdStruct = *(struct date*)rtnData;
    assert(date_eq(rtdStruct, c) && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtdStruct = *(struct date*)rtnData;
    assert(date_eq(rtdStruct, b) && "Cannot pop data\n");
    
    
    rtnData = pop_bytes(&memory);
    rtdStruct = *(struct date*)rtnData;
    assert(date_eq(rtdStruct, b) && "Cannot pop data\n");
    
    rtnData = get_bytes(&memory);
    rtdStruct = *(struct date*)rtnData;
    assert(date_eq(rtdStruct, a) && "Cannot pop data\n");
    
    free_memory(&memory);
    DEBUG_PRINT("Passed basic_struct_test \n");
    TEST_PASSED
}

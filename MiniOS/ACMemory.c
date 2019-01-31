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

const size_t sizet_size = sizeof(size_t);
const size_t overhead_size = sizeof(void*) + sizet_size;

#define element_size_heap(X) *(size_t*)(X - sizet_size)
#define next_element_heap(X) *(void**)(X - overhead_size)





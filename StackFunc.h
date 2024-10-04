#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

#include "hash.h"


#ifndef NDEBUG
#define ON_DEBUG(code) code
#endif



typedef double StackElem_t;
typedef int Canary_t;


enum errors
{
    UNDEFINED_ERROR = 2,
    ALLOC_ERROR = 4,
    DESTRUCTOR_ERROR = 8,
    FILE_CREATION_ERROR = 16,
    MEMSET_FAILURE = 32,
    STACK_UNDERFLOW = 64,
    STACK_PTR_IS_NULL = 128,
    DATA_PTR_IS_NULL = 256,
    STACK_OVERFLOW = 512,
    LEFT_DATA_CANARY_CORRUPTED = 1024,
    RIGHT_DATA_CANARY_CORRUPTED = 2048,
    LEFT_STACK_CANARY_CORRUPTED = 4096,
    RIGHT_STACK_CANARY_CORRUPTED = 8192,
    DATA_CORRUPTED = 16384,
    NO_ERRORS = 0,
};


const int STACK_NAME_MAX = 16;
const int FUNC_NAME_MAX= 32;

typedef struct Stack_t
{
    #ifndef NDEBUG
    Canary_t LStructCanary;

    hash_t HashSum;

    char stkName[STACK_NAME_MAX];
    char file[FILENAME_MAX];
    const char func[FUNC_NAME_MAX];
    const int line;
    #endif

    StackElem_t* data;
    size_t size;
    size_t capacity;


    uint16_t Error;

    #ifndef NDEBUG
    int RStructCanary;
    #endif
} Stack_t;


int StackCtor(Stack_t* stk, size_t stacklen);

int StackPush(Stack_t* stk, StackElem_t element);
int StackPop(Stack_t* stk, StackElem_t* POPelement);
int StackResize(Stack_t* stk, bool resizeflag);

int StackDtor(Stack_t* stk);
int Poising(StackElem_t* data[], size_t lenofNOTPoising, size_t ArrSize);

#endif

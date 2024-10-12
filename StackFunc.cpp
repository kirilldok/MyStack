#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdint.h>
#include "StackFunc.h"
#include "Stackdebug.h"



int StackCtor(Stack_t* stk, size_t stacklen)
{
    if(stacklen == NULL)
    {
        printf("Length of stack must be a natural number!\n");
        return STACK_UNDERFLOW;
    }
    stk->capacity  = stacklen;

    stk->data_with_canaries = (char*)calloc(stk->capacity ON_DEBUG(+ 2), sizeof(StackElem_t));
    stk->data = (StackElem_t*)(stk->data_with_canaries + sizeof(Canary_t));

    if(stk->data_with_canaries == NULL)
    {
        fprintf(stderr, "Dynamic Memory dead\n");
        stk->Error += ALLOC_ERROR;
        return ALLOC_ERROR;
    }

    stk->size = 0;

    #ifndef NDEBUG
        stk->RStructCanary = RightStructCanaryREF;
        stk->LStructCanary = LeftStructCanaryREF;
        *(Canary_t*)(stk->data_with_canaries) = LeftDataCanaryREF;
        ((StackElem_t*)stk->data_with_canaries)[stk->capacity + 1] = RightDataCanaryREF;
        for (size_t i = 0; i < stk->capacity; i++)
        {
            stk->data[i] = Poison;
        }
        stk->HashSum = hash(stk->data, stk->capacity);
    #endif

    StackDump(stk);
    return NO_ERRORS;
}


int StackResize(Stack_t* stk, bool Decrease)
{

    STACK_ASSERT(stk);

    if (Decrease)
    {
        STACK_ASSERT(stk);

        stk->data_with_canaries = (char*)realloc(stk->data, ReallocCoef * stk->capacity *
                                                  sizeof(StackElem_t) ON_DEBUG( + 2 * sizeof(Canary_t)));
        stk->data = (StackElem_t*)(stk->data_with_canaries + sizeof(Canary_t));

        stk->capacity = ReallocCoef * stk->capacity;



        #ifndef NDEBUG
            for (size_t i = stk->size; i < stk->capacity; i++)
            {
                stk->data[i] = Poison;
            }
            ((StackElem_t*)stk->data_with_canaries)[stk->capacity + 1] = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity);
        #endif

        STACK_ASSERT(stk);
        return NO_ERRORS;
    }
    else
    {

        STACK_ASSERT(stk);

        size_t new_capacity = stk->capacity / ReallocCoef;

        stk->data_with_canaries = (char*)realloc(stk->data_with_canaries, new_capacity * sizeof(StackElem_t)
                                                             ON_DEBUG(+ 2 * sizeof(Canary_t)));
        stk->data = (StackElem_t*)(stk->data_with_canaries + sizeof(Canary_t));

        stk->capacity = new_capacity;


        #ifndef NDEBUG
            ((StackElem_t*)stk->data_with_canaries)[stk->capacity + 1] = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity);
        #endif

        STACK_ASSERT(stk);

        return NO_ERRORS;
    }
    STACK_ASSERT((stk));
    return ALLOC_ERROR;
}


int StackPush(Stack_t* stk, StackElem_t element)
{

    STACK_ASSERT(stk);

    if (stk->size  >=  stk->capacity)
    {
        StackResize(stk, true);
    }

    stk->data[stk->size] = element;
    ++stk->size;
    StackDump(stk);


    #ifndef NDEBUG
        for (size_t i = stk->size; i < stk->capacity; i++)
        {
            stk->data[i] = Poison;
        }

        stk->HashSum = hash(stk->data, stk->capacity);
    #endif

    STACK_ASSERT(stk);
    return NO_ERRORS;
}


int StackPop(Stack_t* stk, StackElem_t* Pop_element)
{

    if(Pop_element == NULL)
    {
        printf("Elemment of stack must be StakElem_t type!\n");
        return INPUT_ERROR;
    }

    STACK_ASSERT(stk);

    if (stk->size <= (stk->capacity / 4))
    {
        if (StackResize(stk, false) != 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
    }


    stk->size = stk->size - 1;
    *Pop_element = stk->data[stk->size];



    #ifndef NDEBUG
        stk->data[stk->size] = Poison;
        stk->HashSum = hash(stk->data, stk->capacity);
    #endif

    STACK_ASSERT(stk);

    return NO_ERRORS;
}



int StackDtor(Stack_t* stk)
{

    STACK_ASSERT(stk);

    for(size_t i = 0; i < stk->size; i++)
    {
        stk->data[i] = NULL;

    }

    stk->size = NULL;
    stk->capacity = NULL;

    #ifndef NDEBUG
        stk->LStructCanary = NULL;
        stk->RStructCanary = NULL;
        stk->HashSum = NULL;
        free(stk->data_with_canaries);stk->data_with_canaries = NULL;
    #else
        free(stk->data);stk->data = NULL;
    #endif

    return NO_ERRORS;
}

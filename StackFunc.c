#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdint.h>
#include "StackFunc.h"
#include "Stackdebug.h"




const StackElem_t Poison = -18286663.16668281;





int StackCtor(Stack_t* stk, size_t stacklen)
{
    if(stacklen == NULL)
    {
        printf("Length of stack must be a natural number!\n");
        return 401;
    }

    stk->size = 0;
    stk->capacity  = stacklen;
    stk->data = (StackElem_t*)calloc(stk->capacity, sizeof(StackElem_t));
    printf("stk->data = %p\n", stk->data);


    #ifndef NDEBUG
        stk->RStructCanary = RightStructCanaryREF;
        stk->LStructCanary = LeftStructCanaryREF;
        *(int*)stk->data = LeftDataCanaryREF;
        *(int*)((char*)stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) = RightDataCanaryREF;
        stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif



    Poising(&stk->data, stk->size, stk->capacity);

    return NO_ERRORS;
}


int StackResize(Stack_t* stk, bool resizeflag)
{
    if (resizeflag)
    {
        stk->data = (StackElem_t*)realloc(stk->data, 2 * stk->capacity * sizeof(StackElem_t) ON_DEBUG( + 2 * sizeof(Canary_t)));
        stk->capacity = 2 * stk->capacity;


        #ifndef NDEBUG
            stk->HashSum = hash(stk->data, stk->capacity + 2);
            *(int*)((char*)stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) = RightDataCanaryREF;
        #endif

        return NO_ERRORS;
    }
    else
    {
        printf("stk->data = %p S = %lld\n", stk->data, (long long)(sizeof(StackElem_t) * (stk->capacity / 2) ON_DEBUG( + 2 * sizeof(Canary_t))));
        stk->data = (StackElem_t*)realloc(stk->data, sizeof(StackElem_t) * (stk->capacity / 2) ON_DEBUG( + 2 * sizeof(Canary_t)));
        stk->capacity = stk->capacity / 2;

        #ifndef NDEBUG
            *(int*)((char*)stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity + 2);
        #endif

        return NO_ERRORS;
    }

    return ALLOC_ERROR;
}


int StackPush(Stack_t* stk, StackElem_t element)
{
    if(element == NULL)
    {
        printf("Elemment of stack must be StakElem_t type!");
        return 401;
    }

    if (stk->size  >=  stk->capacity)
    {
        StackResize(stk, true);
    }

    stk->data[stk->size] = element;
    ++stk->size;

    Poising(&stk->data, stk->size, stk->capacity);

    return NO_ERRORS;
}


int StackPop(Stack_t* stk, StackElem_t* POPelement)
{
    // if(POPelement == NULL)
    // {
    //     printf("Elemment of stack must be StakElem_t type!\n");
    //     return 401;
    // }

    stk->size = stk->size - 1;
    *POPelement = stk->data[stk->size];


    if (stk->size <= (stk->capacity / 4))
    {
        if (StackResize(stk, false) != 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
    }

    return NO_ERRORS;
}



int StackDtor(Stack_t* stk)
{
    for(int i = 0; i < stk->size; i++)
    {
        stk->data[i] = NULL;
    }

    stk->size = NULL;
    stk->capacity = NULL;
    stk->LStructCanary = NULL;
    stk->RStructCanary = NULL;
    free(stk);stk = NULL;

    return NO_ERRORS;
}

int Poising(StackElem_t* data[], size_t lenofNOTPoising, size_t ArrSize)
{

    for(int i = lenofNOTPoising; i < ArrSize; ++i)
    {
        (*data)[i] = Poison;
    }

    return NO_ERRORS;
}

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

    stk->data = (StackElem_t*)calloc(stk->capacity, sizeof(StackElem_t));

    if(stk->data == NULL)
    {
        fprintf(stderr, "Dynamic Memory dead\n");
        stk->Error += ALLOC_ERROR;
        return ALLOC_ERROR;
    }

    stk->size = 0;
    stk->capacity  = stacklen;

    //printf("stk->data = %p\n", stk->data);
    //printf("*stk->data = %d\n", *(stk->data));

    #ifndef NDEBUG
        stk->RStructCanary = RightStructCanaryREF;
        stk->LStructCanary = LeftStructCanaryREF;
        *(stk->data) = LeftDataCanaryREF;
        *(stk->data + (stk->capacity) * sizeof(StackElem_t) + sizeof(Canary_t)) = RightDataCanaryREF;
        // printf("RDC = %d\n", RightDataCanaryREF);
        // printf("*stk->data = %d\n", *(stk->data));
        // printf("*stk->datac = %d\n", *(stk->data + (stk->capacity) * sizeof(StackElem_t) + sizeof(Canary_t)));
    #endif

    //printf("poison = %d\n", Poison);
    for (int i = 0; i < stk->capacity; i++)
    {
        *(int*)(stk->data + i * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t))) = Poison;
    }

    #ifndef NDEBUG
        stk->HashSum = hash(stk->data, stk->capacity + 2);
        // printf("hash(stk1) = %zu\n", hash(stk->data, stk->capacity + 2));
        // printf("hash(stk2) = %zu\n", hash(stk->data, stk->capacity + 2));
        // printf("hash(stk3) = %zu\n", hash(stk->data, stk->capacity + 2));
        // printf(" HASH SUM: %zu\n", stk->HashSum);
    #endif

    return NO_ERRORS;
}


int StackResize(Stack_t* stk, bool resizeflag)
{
    STACK_ASSERT(stk);
    if (resizeflag)
    {
        STACK_ASSERT(stk);
        stk->data = (StackElem_t*)realloc(stk->data, ReallocCoef * stk->capacity * sizeof(StackElem_t) ON_DEBUG( + 2 * sizeof(Canary_t)));
        stk->capacity = ReallocCoef * stk->capacity;


        #ifndef NDEBUG
            stk->HashSum = hash(stk->data, stk->capacity + 2);
            *(stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) = RightDataCanaryREF;
        #endif

        STACK_ASSERT(stk);
        return NO_ERRORS;
    }
    else
    {
        STACK_ASSERT(stk);
        //StackDump(stk);
        size_t new_capacity = stk->capacity / ReallocCoef;
        //printf("stack asserted\n");
        //printf("stk->data = %p S = %lld\n", stk->data, (sizeof(StackElem_t) *stk->capacity ON_DEBUG( + 2 * sizeof(Canary_t))));
        printf("slikdhf11111\n");
        StackDump(stk);
        // printf("new_capacity = %lld\n", new_capacity);
        // printf("skbvd = %d\n",  new_capacity * sizeof(StackElem_t));
        // printf("canary = %d\n", sizeof(Canary_t));
        stk->data = (StackElem_t*)realloc( stk->data, new_capacity * sizeof(StackElem_t) ON_DEBUG( + 2 * sizeof(Canary_t)));
        printf("stack realloced\n");
        // StackDump(stk);
        stk->capacity = new_capacity;
        printf("sdwe\n");
        //StackDump(stk);
        // #ifndef NDEBUG
        //     printf("asd\n");
        //     StackDump(stk);
        //     *(stk->data + (stk->capacity) * sizeof(StackElem_t) + sizeof(Canary_t)) = RightDataCanaryREF;
        //     printf("asdbf\n");
        //     stk->HashSum = hash(stk->data, stk->capacity + 2);
        // #endif
        printf("sd1\n");
        StackDump(stk);
        STACK_ASSERT(stk);
        printf("sd\n");
        return NO_ERRORS;
    }
    STACK_ASSERT((stk));
    return ALLOC_ERROR;
}


int StackPush(Stack_t* stk, StackElem_t element)
{
    //printf("sdf\n");

    if(element == NULL)
    {
        printf("Elemment of stack must be StakElem_t type!");
        return INPUT_ERROR;
    }

    STACK_ASSERT(stk);
    //printf("sjdfbf\n");

    if (stk->size  >=  stk->capacity)
    {
        StackResize(stk, true);
    }

    *(stk->data + stk->size * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t))) = element;
    ++stk->size;

    for (int i = stk->size; i < stk->capacity; i++)
    {
        *(int*)(stk->data + i * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t))) = Poison;
    }

    #ifndef NDEBUG
            *(stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) = RightDataCanaryREF;
            stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif

    STACK_ASSERT(stk);
    return NO_ERRORS;
}


int StackPop(Stack_t* stk, StackElem_t* POPelement)
{
    printf("stack pop opened\n");

    if(POPelement == NULL)
    {
        printf("Elemment of stack must be StakElem_t type!\n");
        return INPUT_ERROR;
    }

    STACK_ASSERT(stk);

    stk->size = stk->size - 1;
    *POPelement = *(stk->data + stk->size * sizeof(StackElem_t) ON_DEBUG(+ sizeof(Canary_t)));


    if (stk->size <= (stk->capacity / 4))
    {
        printf("9\n");
        StackDump(stk);
        if (StackResize(stk, false) != 0)
        {
            printf("10\n");
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
    }
    printf("4\n");

    #ifndef NDEBUG
        printf("5\n");
        *(stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) = RightDataCanaryREF;
        stk->HashSum = hash(stk->data, stk->capacity + 2);
        printf("6\n");
    #endif
    printf("7\n");
    STACK_ASSERT(stk);
    printf("8\n");
    return NO_ERRORS;
}



int StackDtor(Stack_t* stk)
{
    printf("stack dtor opened\n");
    STACK_ASSERT(stk);

    for(int i = 0; i < stk->size; i++)
    {
        stk->data[i] = NULL;
    }

    stk->size = NULL;
    stk->capacity = NULL;
    stk->LStructCanary = NULL;
    stk->RStructCanary = NULL;
    free(stk);stk = NULL;

    STACK_ASSERT(stk);
    return NO_ERRORS;
}

// int Poising(StackElem_t* data[], size_t lenofNOTPoising, size_t ArrSize)
// {
//
//     for(int i = lenofNOTPoising; i < ArrSize; ++i)
//     {
//         (*data)[i] = Poison;
//     }
//
//     return NO_ERRORS;
// }

#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdint.h>
#include "StackFunc.h"
#include "Stackdebug.h"



const StackElem_t Poison = -18286663.16668281;
const Canary_t LsCanary = 12000, RsCanary = 12100, LdCanary = 13000, RdCanary = 13100;




int main()
{
    Stack_t stk = { 0 };

    StackCtor(&stk, 32);

    StackPush(&stk, 100);
    printf("stackelement[%lld] = %lg\n", stk.size, stk.data[stk.size-1]);
    StackPush(&stk, 200);
    printf("stackelement[%lld] = %lg\n", stk.size, stk.data[stk.size-1]);

    VoidIntDump(&stk);
    printf("VoidIntDump opened\n");
    StackElem_t  x = 0;

    VoidIntDump(&stk);
    printf("VoidIntDump opened\n");
    StackPop(&stk, &x);

    VoidIntDump(&stk);
    printf("VoidIntDump opened\n");
    printf("x = %lg, stk.size = %lld\n",x, stk.size);

    StackDtor(&stk);

}

// int StackCtor(Stack_t* stk, size_t stacklen)
// {
//     if(stacklen == NULL)
//     {
//         printf("Length of stack must be a natural number!\n");
//         return 401;
//     }
//
//     stk->LstruckCanary = LsCanary;
//     stk->capacity  = stacklen;
//
//     stk->LdataCanary = *((stk->data) - 1 * sizeof(Canary_t)) = LdCanary;
//     stk->data = (StackElem_t*)calloc(stk->capacity, sizeof(StackElem_t));
//     stk->RdataCanary = *((stk->data) + (stk->capacity) * sizeof(size_t)) = RdCanary;
//
//     stk->RstruckCanary = rsCanary;
//
//     Poising(stk->data; stk->size; stk->capacity);
//
//     return 201;
// }


// int StackPush(Stack_t* stk, StackElem_t element)
// {
//     if(element == NULL)
//     {
//         printf("Elemment of stack must be StakElem_t type!");
//         return 401;
//     }
//
//     if (stk->size  >=  stk->capacity)
//     {
//         stk->data = (StackElem_t*)realloc(stk->data, 2 * stk->capacity * sizeof(StackElem_t));
//         stk->capacity = 2 * stk->capacity;
//     }
//
//     stk->data[stk->size] = element;
//     ++stk->size;
//
//     Poising(stk->data; stk->size; stk->capacity);
//
//     return 200;
// }


// int StackPop(Stack_t* stk, StackElem_t* POPelement)
// {
//     // if(POPelement == NULL)
//     // {
//     //     printf("Elemment of stack must be StakElem_t type!\n");
//     //     return 401;
//     // }
//
//     stk->size = stk->size - 1;
//     *POPelement = stk->data[stk->size];
//
//
//     if (stk->size <= (stk->capacity / 4))
//     {
//         stk->data = (StackElem_t*)realloc(stk->data, sizeof(StackElem_t) * (stk->capacity / 4));
//         stk->capacity = stk->capacity / 4;
//     }
//
//     return 200;
// }



// int StackDtor(Stack_t* stk)
// {
//
//
//     for(int i = 0; i < stk->size; i++)
//     {
//         stk->data[i] = NULL;
//     }
//
//     stk->size = NULL;
//     stk->capacity = NULL;
//     stk->LstackCanary = NULL;
//     stk->RstackCanary = NULL;
//     FREE(stk);
// }




//
// int Poising(StackElem_t* data[], size_t lenofNOTPoising, size_t ArrSize)
// {
//
//     for(int i = lenofNOTPoising; i < ArrSize; ++i)
//     {
//         data[i] = Poison;
//     }
//
//     return 200;
// }

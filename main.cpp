#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdint.h>
#include "StackFunc.h"
#include "Stackdebug.h"



int main()
{
    #ifndef NDEBUG
    FILE* clean = fopen("log.txt", "w+");
    fclose(clean);
    #endif

    Stack_t stk = { 0 };
    StackCtor(&stk, 32);

    StackPush(&stk, 100.5);
    StackDump(&stk);

    StackPush(&stk, 200.34);
    StackDump(&stk);

    StackElem_t  x = 0;


    StackPush(&stk, 300.21);
    StackDump(&stk);

    StackPop(&stk, &x);
    printf("x = %lg, stk.size = %lu\n", x, stk.size);
    StackDump(&stk);

    StackDtor(&stk);
    return 0;


}

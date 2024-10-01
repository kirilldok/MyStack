#include<stdio.h>
#include<string.h>
#include<malloc.h>

typedef double StackElem_t;

typedef struct Stack_t
{
    StackElem_t* data;
    size_t size;
    size_t capacity;
}Stack_t;

int StackCtor(Stack_t* stk, size_t len);
int StackPush(Stack_t* stk, StackElem_t element);
int StackPop(Stack_t* stk, StackElem_t* POPelement);


     
int main()  
{
    Stack_t stk = { 0 };

    StackCtor(&stk, 32);//make i nput test 1

    StackPush(&stk, 100);//функция увеличение стека при переполнении
    printf("stackelement[%lld] = %lg\n", stk.size, stk.data[stk.size-1]);
    StackPush(&stk, 200);
    printf("stackelement[%lld] = %lg\n", stk.size, stk.data[stk.size-1]);

    StackElem_t  x = NULL;
    StackPop(&stk, &x);  
    printf("x = %lg, stk.size = %lld\n",x, stk.size);

//     StackDtor(&stk);

}

int StackCtor(Stack_t* stk, size_t stacklen)
{
    if(stacklen == NULL)
    {
        printf("Length of stack must be a natural number!\n");
        return 401;
    }

    stk->capacity  = stacklen;
    stk->data = (StackElem_t*)calloc(stk->capacity, sizeof(StackElem_t));

    return 201;
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
        stk->data = (StackElem_t*)realloc(stk->data, 2 * stk->capacity * sizeof(StackElem_t));
        stk->capacity = 2 * stk->capacity;
    }

    stk->data[stk->size] = element;
    ++stk->size;

    return 200;
}


int StackPop(Stack_t* stk, StackElem_t* POPelement)
{
    if(POPelement == NULL)
    {
        printf("Element of stack must be StakElem_t type!\n");
        return 401;
    }

    stk->size = stk->size - 1;
    *POPelement = stk->data[stk->size];


    if (stk->size <= (stk->capacity / 4))
    {
        stk->data = (StackElem_t*)realloc(stk->data, sizeof(StackElem_t) * (stk->capacity / 4));
        stk->capacity = stk->capacity / 4;
    }  

    return 200;
}

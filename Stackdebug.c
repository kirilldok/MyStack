#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <cstdint>

#include "Stackdebug.h"
#include "StackFunc.h"
#include "hash.h"

void StackAssert(Stack_t* stk, const char* stkName, const char* file, const char* func, int line)
{

    if(stk == NULL)
    {

        fprintf(stderr, "Stack pointer is NULL");
        assert(0);
    }

    if((stk->Error = StackError(stk)) != 0)
    {

        VoidIntDump(stk);
        fprintf(stderr, "Emergency exit, stack dumped into log.txt");
        assert(0);
    }

}



int StackError(Stack_t* stk)
{
    size_t errFlag = 0;
    if(stk == NULL)
    {
        fprintf(stderr, "Stack pointer is NULL\n");
        errFlag += STACK_PTR_IS_NULL;
    }

    if(stk->data == NULL)
    {
        fprintf(stderr, "Data pointer is NULL\n");
        errFlag += DATA_PTR_IS_NULL;
    }

    if(stk->size > stk->capacity)
    {
        fprintf(stderr, "Stack overflow\n");
        errFlag += STACK_OVERFLOW;
    }

    if(stk->size > UNLIKELY_STACKSIZE)
    {
        fprintf(stderr, "Stack has negative size (stack underflow)\n");
        errFlag += STACK_UNDERFLOW;
    }

    if(*(int*)((char*)stk->data) != LeftDataCanaryREF)
    {
        errFlag += LEFT_DATA_CANARY_CORRUPTED;
    }

    if(*(int*)((char*)stk->data + (stk->capacity + 1)*sizeof(StackElem_t)) != RightDataCanaryREF)
    {
        errFlag += RIGHT_DATA_CANARY_CORRUPTED;
    }

    if(stk->LStructCanary != LeftStructCanaryREF)
    {
        errFlag += LEFT_STACK_CANARY_CORRUPTED;
    }

    if(stk->RStructCanary != RightStructCanaryREF)
    {
        errFlag += RIGHT_STACK_CANARY_CORRUPTED;
    }

    if(stk->HashSum != hash(stk->data, stk->capacity + 2))
    {
        errFlag += DATA_CORRUPTED;
    }

    return errFlag;
}



int VoidIntDump(Stack_t* stk)
{
    printf("VoidIntDump opened\n");
    printf("stk = %p\n", stk);
    printf("########################## STACK INFO ##########################\n"
    "##CALLER STACK NAME: %s\n"
    "## CALLER STACK POINTER: %p\n"
    "## CALLER FILE    : %s\n"
    "## CALLER FUNCTION: %s\n"
    "## LINE          = %d\n"
    "## STACK SIZE    = %zu\n"
    "## STACK CAPACIY = %zu\n"
    "## STACK DATA: %p\n", stk->stkName, stk, stk->file, stk->func, stk->line, stk->size, stk->capacity, (char*)stk->data);

    printf("### LEFT CANARY: %d\n", *(int*)((char*)stk->data));


    if(stk == NULL) printf("Dump NULL\n");

    printf("Dump != NULL\n");
    FILE* log = fopen("log.txt", "ab");

    printf("log.txt opened\n");

    if(log == NULL)
    {

        stk->Error = FILE_CREATION_ERROR;
        printf("stk->ERROR = %lld\n", stk->Error);

        fclose(log);

        return FILE_CREATION_ERROR;

    }



    fprintf(log,
    "########################## STACK INFO ##########################\n"
    "##CALLER STACK NAME: %s\n"
    "## CALLER STACK POINTER: %p\n"
    "## CALLER FILE    : %s\n"
    "## CALLER FUNCTION: %s\n"
    "## LINE          = %d\n"
    "## STACK SIZE    = %zu\n"
    "## STACK CAPACIY = %zu\n"
    "## STACK DATA: %p\n", stk->stkName, stk, stk->file, stk->func, stk->line, stk->size, stk->capacity, (char*)stk->data);



    fprintf(log, "## DATA BUFFER: \n");

    fprintf(log, "### LEFT CANARY: %d\n", *(int*)((char*)stk->data));

    for(size_t i = 1; i < stk->capacity + 1; i++)
    {

        fprintf(log, "# [%d]  (%zu)\n", *(int*)((char*)stk->data + i*sizeof(StackElem_t)), i);

    }

    fprintf(log, "### RIGHT CANARY: %d\n", *(int*)((char*)stk->data + (stk->capacity + 1)*sizeof(StackElem_t)));

    fprintf(log, "## HASH SUM: %zu\n", stk->HashSum);
    fprintf(log, "## ERRORS: ");

    uint16_t byte = 2;

    for(size_t i = 0; i < sizeof(uint16_t)*8; i++)
    {

        fprintf(log, "%hu", (stk->Error & byte) ? 1 : 0);

        byte *= 2;

    }

    fprintf(log, "\n################################################################\n\n\n\n");
    fclose(log);

    return NO_ERRORS;
}


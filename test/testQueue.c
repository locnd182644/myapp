#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define BUFF_SIZE 100
#define ElemNum 3

char* ptr;
char* fetch;
char buff[BUFF_SIZE];

int main()
{
    init_queue();

    for(uint8_t i = 0; i < ElemNum; i++)
    {
        ptr = (char*) malloc(BUFF_SIZE * sizeof(char));
        printf("Nhap Element %d: ", i+1);
        scanf("%s", buff);
        strncpy(ptr, buff, BUFF_SIZE);
        enQueue(ptr);
    }

    for(uint8_t i = 0; i < ElemNum; i++)
    {
        fetch = deQueue();
        printf("Element %d: %s\n", i+1, fetch);
        free(fetch);
    }

    return 0;
}
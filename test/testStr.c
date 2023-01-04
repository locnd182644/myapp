#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 100
#define ElemNum 3

char *ptr;
char *fetch;
char buff[10];

int main()
{
    int val = 0;
    printf("Nhap number: ");
    scanf("%s", buff);
    for (uint8_t i = 0; i < strlen(buff); i++)
    {
        val *= 10;
        val += (uint8_t)buff[i] - 48;
    }
    printf("Convert string to int: %d\n", val);
    return 0;
}
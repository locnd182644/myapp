#ifndef _QUEUE_
#define _QUEUE_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void init_queue();
bool isFull();
bool isEmpty();
void enQueue(uint8_t*);
uint8_t* deQueue();
void display();
int countElement();
void displayStruct(uint8_t *);

#endif
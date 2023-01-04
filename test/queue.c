/*******************************************************************************
 *
 *  FILENAME: queue.c
 *
 *  PURPOSE:
 *
 *  FILE REFERENCES:
 *
 *  Name            I/0                 Description
 *  ---------       ---                 -----------
 *  none
 *
 *
 *  EXTERNAL VARIABLES:
 *
 *  Source:  queue.h
 *
 *  Name                Type            I/0     Description
 *  ---------           ----            ---     ----------------
 *
 *
 *  EXTERNAL REFERENCES:
 *
 *  Name                                Description
 *  ----------------------              ----------------------------------
 *
 *
 *  ABNORMAL TERMINATION CONDITIONS, ERROR AND  WARNING MESSAGES: none
 *
 *
 *  ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS: none
 *
 *
 *  NOTES: none
 *
 *
 *  REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:  none
 *
 *
 *  DEVELOPMENT HISTORY:
 *  Date        Author              Change      Release     Description of Change
 *                                  ID
 *  --------    --------            -------     -------     ---------------------
 *
 *
 *  ALGORITHM (PDL)
 *
 *
 *********************************************************************************/

#include "queue.h"

#define _DB_LOG_
#define QUEUE_SIZE 5

uint8_t *package[QUEUE_SIZE];
int front, rear;

/*************************************************************************
 *
 *  FUNCTION NAME:  init_queue
 *
 *  @brief: init function
 *
 *  AGRUMENTS LIST: none
 *
 *  RETURN VALUE: none
 *
 **************************************************************************/
void init_queue()
{
    front = -1;
    rear = -1;
}

/*************************************************************************
 *
 *  FUNCTION NAME:  isFull
 *
 *  @brief: Check if the queue is full
 *
 *  AGRUMENTS LIST: none
 *
 *  RETURN VALUE: bool
 *
 **************************************************************************/
bool isFull()
{
    if (front == 0 && rear == QUEUE_SIZE - 1)
    {
        return true;
    }
    if (front == rear + 1)
    {
        return true;
    }
    return false;
}

/*************************************************************************
 *
 *  FUNCTION NAME:  isEmpty
 *
 *  @brief: Check if the queue is empty
 *
 *  AGRUMENTS LIST: none
 *
 *  RETURN VALUE: bool
 *
 **************************************************************************/
bool isEmpty()
{
    if (front == -1)
        return true;
    else
        return false;
}

/*************************************************************************
 *
 *  FUNCTION NAME:  enQueue
 *
 *  @brief: Adding an element to the queue
 *
 *  AGRUMENTS LIST:
 *  AGRUMENTS       TYPE            I/O     DESCRIPTION
 *  ---------       ----            ---     -----------
 *  element         DataCore        I       Element to be
 *                                          added to the queue
 *
 *  RETURN VALUE: void
 *
 **************************************************************************/
void enQueue(uint8_t *element)
{
    if (isFull())
    {
        printf("Queue is full");
    }
    else
    {
        if (front == -1)
        {
            front = 0;
        }
        rear = (rear + 1) % QUEUE_SIZE;
        package[rear] = element;
#ifdef _DB_LOG_
        printf("Count after en: %d\n", countElement());
        printf("Front -> %d\n", front);
        printf("Rear -> %d\n", rear);
#endif
    }
}

/*************************************************************************
 *
 *  FUNCTION NAME:  deQueue
 *
 *  @brief: Remove an element from the queue
 *
 *  AGRUMENTS LIST: none
 *
 *  RETURN VALUE: DataCore
 *
 **************************************************************************/
uint8_t *deQueue()
{
    uint8_t *element = NULL;
    if (isEmpty())
    {
        printf("Queue is empty\n");
        return element;
    }
    else
    {
        element = package[front];
        /* Queue has only one element,
        -> we reset the queue after deleting it. */
        if (front == rear)
        {
            front = -1;
            rear = -1;
        }
        else
        {
            front = (front + 1) % QUEUE_SIZE;
        }
#ifdef _DB_LOG_
        printf("Count after de: %d\n", countElement());
        printf("Front -> %d\n", front);
        printf("Rear -> %d\n", rear);
#endif
        return (element);
    }
}

void display()
{
    // Function to display status of Circular Queue
    if (isEmpty())
    {
        printf("Empty Queue\n");
        return;
    }
    printf("\nElements in Circular Queue are: \n");
    if (rear >= front)
    {
        for (int i = front; i <= rear; i++)
            displayStruct(package[i]);
    }
    else
    {
        for (int i = front; i < QUEUE_SIZE; i++)
            displayStruct(package[i]);

        for (int i = 0; i <= rear; i++)
            displayStruct(package[i]);
    }
}

int countElement()
{
    if (isEmpty())
    {
        printf("Empty Queue\n");
        return 0;
    }
    if (front <= rear)
    {
        return rear - front + 1;
    }
    else
    {
        return rear + QUEUE_SIZE - front + 1;
    }
}

void displayStruct(uint8_t *s)
{
}

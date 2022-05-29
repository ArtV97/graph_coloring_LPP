#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct info {
    int val;
    struct info *next;
} Info;


typedef struct stack {
    Info *top;
    int len;
} Stack;

Stack *stack_init();
void free_stack(Stack *s);
void push(Stack *s, int val);
int pop(Stack *s);
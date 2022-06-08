#include "stack.h"

Stack *stack_init() {
    Stack *s = (Stack *)malloc(sizeof(Stack));

    s->top = NULL;
    s->len = 0;

    return s;
}


void push(Stack *s, int val) {
    Info *new = (Info*)malloc(sizeof(Info));
    new->val = val;

    new->next = s->top;
    s->top = new;
    s->len++;
}


int pop(Stack *s) {
    Info *aux = s->top;
    int val = aux->val;
    
    s->top = aux->next;
    
    s->len--;
    free(aux);

    return val;
}


void free_stack(Stack *s) {
    if (s->len != 0) {
        Info *aux, *aux2;
        aux = s->top;
        while (aux) {
            aux2 = aux->next;

            free(aux);
            aux = aux2;
        }
    }
    
    free(s);
}

// int main() {
//     Stack *s = stack_init();
//     push(s, 13);
//     push(s, 7);
//     push(s, -1);
//     push(s, 2);

//     int n = s->len;
//     for (int i = 0; i < n; i++) {
//         fprintf(stderr, "POP: %d, Len: %d\n", pop(s), s->len);
//     }

//     free_stack(s);
//     return 0;
// }
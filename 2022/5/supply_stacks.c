#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_STACK_ELEMENTS 128
#define NUM_STACKS 9
#define MAX_BUFFER_SIZE 1024

typedef struct Stack {
    char *elements;
    int num_elements;
} Stack;

void push_front(Stack *s, char c);
void push_back(Stack *s, char c);
char pop(Stack *s);
int get_next_num(char *s, int start);
void print_stacks(Stack *stacks);

int main(int argc, char *argv[]) {
    size_t buf_size = MAX_BUFFER_SIZE;
    char *line = NULL;

    /* Open input */
    FILE *input = fopen("input.txt", "r");

    /* Get the starting arrangement of the stacks */
    Stack *stacks = malloc(sizeof(Stack) * NUM_STACKS);
    memset(stacks, 0, sizeof(Stack) * NUM_STACKS);
    for (int i = 0; i < NUM_STACKS; i++) {
        stacks[i].elements = malloc(128);
    }
    while ( (getline(&line, &buf_size, input) != -1) && (*line != '\n') ) {
        // Push the existing elements onto their respective stacks
        if (*line == ' ') {
            continue;
        }
        for (int i = 0; i < NUM_STACKS; i++) {
            // The relevant chars are every 4 chars into the line
            if (line[1 + (4 * i)] != ' ') {
                push_front(&stacks[i], line[1 + (4 * i)]);
            }
        }
    }

    /* Print the starting stack arrangement */
    printf("\tSTARTING ARRANGEMENT\n");
    print_stacks(stacks);

    /* Carry out the rearrangement procedures */
    int num_moved = 0;
    int from = 0;
    int to = 0;
    char moved_elem = 0;
    Stack *temp_stack = malloc(sizeof(Stack)); // temporary stack used to transfer multiple at a time
    temp_stack->elements = malloc(128);
    while ( (getline(&line, &buf_size, input) != -1) ) {
        // Get the number of moved elements (starts at indx 5)
        num_moved = get_next_num(line, 5);
        // Get the stack elem(s) is moved from
        from = get_next_num(line, 12);
        // Get the stack elem(s) is moved to
        to = get_next_num(line, 17);
        // Move the element
        temp_stack->num_elements = 0;
        for (num_moved; num_moved > 0; num_moved--) {
            moved_elem = pop(&stacks[from - 1]);
            // push the element to the back of the temporary stack
            push_back(temp_stack, moved_elem);
            
        }
        for (temp_stack->num_elements; temp_stack->num_elements > 0; ) {
            moved_elem = pop(temp_stack);
            push_back(&stacks[to - 1], moved_elem);
        }
    }

    /* Print the ending stack arrangement */
    printf("\tENDING_ARRANGEMENT\n");
    print_stacks(stacks);

    free(temp_stack->elements);
    free(temp_stack);
    for (int i = 0; i < NUM_STACKS; i++) {
        free(stacks[i].elements);
    }
    free(stacks);
    free(line);
    fclose(input);

    return 0;
}

void push_front(Stack *s, char c) {
    for (int i = s->num_elements; i >= 0; i--) {
        s->elements[i + 1] = s->elements[i];
    }
    s->elements[0] = c;
    s->num_elements++;
}

void push_back(Stack *s, char c) {
    s->elements[s->num_elements] = c;
    s->num_elements++;
}

char pop(Stack *s) {
    char ret = s->elements[s->num_elements - 1];
    s->elements[s->num_elements - 1] = 0;
    s->num_elements--;
    return ret;
}

int get_next_num(char *s, int start) {
    int ret = 0;
    int num_multiples_of_ten = 0;
    int indx = start;
    // Move index to beginning of number
    for (; isdigit(s[indx]) == 0; indx++) {}

    for (; isdigit(s[indx]) != 0; indx++) {
        ret *= (10 * num_multiples_of_ten);
        ret += s[indx] - '0';
        num_multiples_of_ten++;
    }

    return ret;
}

void print_stacks(Stack *stacks) {
    int max_height = 0;
    for (int i = 0; i < NUM_STACKS; i++) {
        if (stacks[i].num_elements > max_height) {
            max_height = stacks[i].num_elements;
        }
    }

    int line_idx;
    for (int i = max_height; i > 0; i--) {
        line_idx = 0;
        for (int j = 0; j < NUM_STACKS; j++) {
            // The first bracket of each stack starts at indx 4
            line_idx = 4 * j;
            if (stacks[j].num_elements >= i) {
                putchar('[');
                putchar(stacks[j].elements[i-1]);
                putchar(']');
            } else {
                putchar(' ');
                putchar(' ');
                putchar(' ');
            }
            putchar(' ');
        }
        putchar('\n');
    }

    for (int i = 0; i <= NUM_STACKS; i++) {
        putchar(' ');
        putchar((i + 1) + '0');
        putchar(' ');
        putchar(' ');
    }
    putchar('\n');
}

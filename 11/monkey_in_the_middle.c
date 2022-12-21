#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define NUM_MONKEYS 8

typedef enum Operation {
    ADDITION,
    MULTIPLICATION,
    SQUARE,
} Operation;

typedef struct ItemNode ItemNode;

struct ItemNode {
    uint64_t worry_level;
    ItemNode* next;
};

typedef struct ItemList {
    ItemNode* first;
    ItemNode* last;
    int num_items;
} ItemList;

typedef struct MonkeyInfo MonkeyInfo;

struct MonkeyInfo {
    ItemList* starting_items;
    Operation operation;
    int operand;         // Value that the item will be combined with in Operation
    int test;            // Int that divisibility will be checked against
    int true_monkey;     // Monkey item is thrown to if test is true
    int false_monkey;    // Monkey item is thrown to if test is false
    uint64_t items_inspected;
};

ItemNode* pop_front(ItemList* list);
void append(ItemList* list, ItemNode* item);
ItemNode* create_item_node(uint64_t worry_level);
void destroy_item_node(ItemNode* item_node);
ItemList* create_item_list();
void destroy_item_list(ItemList* item_list);
void do_round(MonkeyInfo monkeys[], MonkeyInfo* cur_monkey, int relief_factor, uint64_t lcm);
uint64_t get_monkey_business(MonkeyInfo monkeys[]);

int
main(int argc, char *argv[])
{
    FILE* input = fopen("input.txt", "r");
    size_t buf_size = 128;
    char* line = calloc(buf_size, sizeof(char));

    /* Initialize the monkeys */

    uint64_t worry_level;
    MonkeyInfo monkeys_part1[NUM_MONKEYS] = { 0 };
    MonkeyInfo monkeys_part2[NUM_MONKEYS] = { 0 };
    for (int i = 0; i < NUM_MONKEYS; i++) {
        getline(&line, &buf_size, input);

        // Get the starting items
        monkeys_part1[i].starting_items = create_item_list();
        monkeys_part2[i].starting_items = create_item_list();
        getline(&line, &buf_size, input);
        int FIRST_ITEM_IDX = 17;
        char *temp_line = &line[FIRST_ITEM_IDX];
        while ( ( worry_level = strtol(&temp_line[1], &temp_line, 10) ) != 0 ) {
            ItemNode* node_1 = create_item_node(worry_level);
            ItemNode* node_2 = create_item_node(worry_level);
            append(monkeys_part1[i].starting_items, node_1);
            append(monkeys_part2[i].starting_items, node_2);
        }

        // Get the operand and operation
        int OPERATION_IDX = 23;
        int OPERAND_IDX = 25;
        getline(&line, &buf_size, input);
        int operand = strtol(&line[OPERAND_IDX], NULL, 10); // Get operand
        monkeys_part1[i].operand = operand;
        monkeys_part2[i].operand = operand;
        if (operand == 0) { // If there is no numerical operand in input, Operation is SQUARE
            monkeys_part1[i].operation = SQUARE;
            monkeys_part2[i].operation = SQUARE;
        } else if (line[OPERATION_IDX] == '+') {
            monkeys_part1[i].operation = ADDITION;
            monkeys_part2[i].operation = ADDITION;
        } else {
            monkeys_part1[i].operation = MULTIPLICATION;
            monkeys_part2[i].operation = MULTIPLICATION;
        }

        // Get test value
        int TEST_VAL_IDX = 21;
        getline(&line, &buf_size, input);
        monkeys_part1[i].test = strtol(&line[TEST_VAL_IDX], NULL, 10);
        monkeys_part2[i].test = strtol(&line[TEST_VAL_IDX], NULL, 10);

        // Get the test true value
        int TRUE_MNKY_IDX = 29;
        getline(&line, &buf_size, input);
        monkeys_part1[i].true_monkey = strtol(&line[TRUE_MNKY_IDX], NULL, 10);
        monkeys_part2[i].true_monkey = strtol(&line[TRUE_MNKY_IDX], NULL, 10);

        // Get the test false value
        int FALSE_MNKY_IDX = 30;
        getline(&line, &buf_size, input);
        monkeys_part1[i].false_monkey = strtol(&line[FALSE_MNKY_IDX], NULL, 10);
        monkeys_part2[i].false_monkey = strtol(&line[FALSE_MNKY_IDX], NULL, 10);

        getline(&line, &buf_size, input);
        memset(line, 0, buf_size);
    }

    free(line);
    fclose(input);

    uint64_t least_common_multiple = 1;
    for (int i = 0; i < NUM_MONKEYS; i++) {
        least_common_multiple *= monkeys_part1[i].test;
    }

    /* Carry out 20 rounds of monkey business */

    for (int round = 0; round < 20; round++) {
        for (int i = 0; i < NUM_MONKEYS; i++) {
            do_round(monkeys_part1, &monkeys_part1[i], 3, least_common_multiple);
        }
    }

    /* Carry out 10000 rounds of monkey business */

    for (int round = 0; round < 10000; round++) {
        for (int i = 0; i < NUM_MONKEYS; i++) {
            do_round(monkeys_part2, &monkeys_part2[i], 1, least_common_multiple);
        }
    }

    /* Get the total monkey business */

    printf("Part 1 monkey business: %lu\n", get_monkey_business(monkeys_part1));
    printf("Part 2 monkey business: %lu\n", get_monkey_business(monkeys_part2));

    for (int i = 0; i < NUM_MONKEYS; i++) {
        destroy_item_list(monkeys_part1[i].starting_items);
        destroy_item_list(monkeys_part2[i].starting_items);
    }

    return EXIT_SUCCESS;
}

ItemNode*
pop_front(ItemList* list)
{
    if (list->num_items == 0) {
        return NULL;
    }

    ItemNode* ret = list->first;
    list->first = ret->next;
    if (ret->next == NULL) {
        list->last = NULL;
    }
    list->num_items--;
    ret->next = NULL;

    return ret;
}

void
append(ItemList* list, ItemNode* item)
{
    if (list->num_items == 0) {
        list->first = item;
    } else {
        list->last->next = item;
    }
    list->last = item;
    list->num_items++;
}

ItemNode*
create_item_node(uint64_t worry_level)
{
    ItemNode* ret = calloc(1, sizeof(ItemNode));
    ret->worry_level = worry_level;
    return ret;
}

void
destroy_item_node(ItemNode* item_node)
{
    free(item_node);
}

ItemList*
create_item_list()
{
    ItemList* ret = calloc(1, sizeof(ItemList));
    ret->num_items = 0;
    return ret;
}

void
destroy_item_list(ItemList* item_list)
{
    ItemNode* cur_node = item_list->first;
    ItemNode* temp;
    while (cur_node != NULL) {
        temp = cur_node->next;
        destroy_item_node(cur_node);
        cur_node = temp;
    }
    free(item_list);
}

void do_round(MonkeyInfo monkeys[], MonkeyInfo* cur_monkey, int relief_factor, uint64_t lcm)
{
    // Inspect each item
    ItemNode* cur_item = pop_front(cur_monkey->starting_items);
    while (cur_item != NULL) {
        cur_monkey->items_inspected++;
        // Increase worry level
        if (cur_monkey->operation == SQUARE) {
            cur_item->worry_level *= cur_item->worry_level;
        } else if (cur_monkey->operation == ADDITION) {
            cur_item->worry_level += cur_monkey->operand;
        } else {
            cur_item->worry_level *= cur_monkey->operand;
        }
        // Decrease worry level due to boredom (Part 1)
        if (relief_factor > 1) {
            cur_item->worry_level /= relief_factor;
        }
        // Ensure that the item's value stays within the LCM of all monkeys (Part 2)
        else {
            cur_item->worry_level %= lcm;
        }
        // Run test
        MonkeyInfo* next_monkey;
        if (cur_item->worry_level % cur_monkey->test == 0) {
            // Throw to true monkey
            next_monkey = &monkeys[cur_monkey->true_monkey];
        } else {
            // Throw to false monkey
            next_monkey = &monkeys[cur_monkey->false_monkey];
        }
        append(next_monkey->starting_items, cur_item);
        cur_item = pop_front(cur_monkey->starting_items);
    }
}

uint64_t get_monkey_business(MonkeyInfo monkeys[])
{
    uint64_t first = 0;
    uint64_t second = 0;

    for (int i = 0; i < NUM_MONKEYS; i++) {
        uint64_t num_inspected = monkeys[i].items_inspected;
        if (num_inspected > first) {
            second = first;
            first = num_inspected;
        } else if (num_inspected > second) {
            second = num_inspected;
        }
    }

    return first * second;
}

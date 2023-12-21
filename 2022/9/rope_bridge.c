#include <stdlib.h>
#include <stdio.h>

#define UP 'U'
#define DOWN 'D'
#define RIGHT 'R'
#define LEFT 'L'

typedef struct Coordinate {
    int x;
    int y;
} Coordinate;

typedef struct Node Node;

struct Node {
    Coordinate *coordinate;
    Node *next;
};

typedef struct VisitedList {
    Node *head;
    Node *tail;
    int num_visited;
} VisitedList;

typedef enum BOOL {
    FALSE,
    TRUE,
} BOOL;

BOOL have_visited(VisitedList *list, Coordinate *coor);
void add_coordinate(VisitedList *list, Coordinate *coor);
int get_distance(Coordinate *c1, Coordinate *c2);
void move_knot(Coordinate *prev_c, Coordinate *cur_c);

void destroy_list(VisitedList *list);

int main(int argc, char *argv[]) {
    // Open the file
    FILE *input = fopen("input.txt", "r");

    // Starting coordinates of the head and tail
    Coordinate head = { 0, 0 };
    Coordinate tail = { 0, 0 };

    // Linked list to track visited coordinates
    VisitedList *visited = calloc(1, sizeof(VisitedList));
    add_coordinate(visited, &tail);

    size_t buf_size = 16; // Lines in input are short
    char *line = calloc(buf_size, sizeof(char));

    /* Determine number of coordinates visited by tail at least once */

    char direction;
    int num_moves;
    while (getline(&line, &buf_size, input) != -1) {
        // Get the direction
        direction = line[0];
        // Get the number of moves
        num_moves = strtol(&line[2], NULL, 10);
        
        for (num_moves; num_moves > 0; num_moves--) {
            // Adjust the head's coordinates
            switch (direction) {
                case UP:
                    head.y++;
                    break;
                case DOWN:
                    head.y--;
                    break;
                case RIGHT:
                    head.x++;
                    break;
                case LEFT:
                    head.x--;
                    break;
                default:
                    printf("Invalid move\n");
            }

            // If the head and tail are now more than 1 space apart, move tail
            if (get_distance(&head, &tail) > 1) {
                move_knot(&head, &tail);
                add_coordinate(visited, &tail);
            }
        }
    }

    printf("Number of positions visited by tail at least once: %d\n", visited->num_visited);

    destroy_list(visited);
    fclose(input);

    /* Determine number of coordinates visited by knot 9 (new tail) at least once */

    input = fopen("input.txt", "r");

    Coordinate rope[10] = { 0 }; // Head is rope[0], tail is rope[9]

    visited = calloc(1, sizeof(VisitedList));
    add_coordinate(visited, &rope[9]);

    char temp_direction;
    while (getline(&line, &buf_size, input) != -1) {
        // Get the direction
        direction = line[0];
        // Get the number of moves
        num_moves = strtol(&line[2], NULL, 10);

        for (num_moves; num_moves > 0; num_moves--) {
            // Adjust the head's coordinates
            switch (direction) {
                case UP:
                    rope[0].y++;
                    break;
                case DOWN:
                    rope[0].y--;
                    break;
                case RIGHT:
                    rope[0].x++;
                    break;
                case LEFT:
                    rope[0].x--;
                    break;
                default:
                    printf("Invalid move\n");
            }

            // printf("Head moved %c to (%d, %d)\n", direction, rope[0].x, rope[0].y);
            temp_direction = direction;

            // Adjust each knot's coordinates
            int x_dist, y_dist;
            for (int i = 1; i < 10; i++) {
                if (get_distance(&rope[i-1], &rope[i]) > 1) {
                    move_knot(&rope[i-1], &rope[i]);

                    if (i == 9) {
                        add_coordinate(visited, &rope[i]);
                    }

                    // printf("Knot %d moved to (%d, %d)\n", i, rope[i].x, rope[i].y);
                } else {
                    break;
                }
            }
        }
    }

    printf("Number of positions visited by knot 9 (tail) at least once: %d\n", visited->num_visited);

    free(line);
    destroy_list(visited);
    fclose(input);

    return EXIT_SUCCESS;
}

BOOL have_visited(VisitedList *list, Coordinate *coor) {
    // A hash set would've been more optimal but I don't have a hash algorithm
    // readily available
    Node *cur_node = list->head;
    while (cur_node != NULL) {
        if (cur_node->coordinate->x == coor->x && cur_node->coordinate->y == coor->y) {
            return TRUE;
        }
        cur_node = cur_node->next;
    }
    return FALSE;
}

void add_coordinate(VisitedList *list, Coordinate *coor) {
    // "How many positions does the tail of the rope visit at least once?"
    if (have_visited(list, coor) == TRUE) {
        return;
    }

    Node *new_node = calloc(1, sizeof(Node));
    new_node->coordinate = calloc(1, sizeof(Coordinate));
    new_node->coordinate->x = coor->x;
    new_node->coordinate->y = coor->y;

    if (list->num_visited == 0) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->num_visited++;
}

int get_distance(Coordinate *c1, Coordinate *c2) {
    int x_dist = abs(c1->x - c2->x);
    int y_dist = abs(c1->y - c2->y);
    int dist;

    if (x_dist > 1) {
        dist = x_dist;
    } else {
        dist = y_dist;
    }
    return dist;
}

void move_knot(Coordinate *prev_c, Coordinate *cur_c) {
    int x_dist = prev_c->x - cur_c->x;
    int y_dist = prev_c->y - cur_c->y;

    if (x_dist == 0) {        // Same x, move y
        cur_c->y += y_dist > 0 ? 1 : -1;
    } else if (y_dist == 0) { // Same y, move x
        cur_c->x += x_dist > 0 ? 1 : -1;
    } else {                  // Move both x and y
        cur_c->x += x_dist > 0 ? 1 : -1;
        cur_c->y += y_dist > 0 ? 1 : -1;
    }
}

void destroy_list(VisitedList *list) {
    Node *cur_node = list->head;
    Node *next;
    while (cur_node != NULL) {
        next = cur_node->next;
        free(cur_node->coordinate);
        free(cur_node);
        cur_node = next;
    }
    free(list);
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

const char VISITED = 'X';
const char NOT_VISITED = '.';
size_t NUM_ROWS;
size_t NUM_COLS;

typedef enum Bool
{
    FALSE,
    TRUE,
} Bool;

typedef struct Point Point;
struct Point
{
    int row;
    int col;
    size_t elevation;
    size_t distance;
    size_t num_neighbors;
    Point* neighbors[4];
    Point* prev;
};

typedef struct Node Node;
struct Node
{
    Point* point;
    Node* prev;
    Node* next;
};

typedef struct List
{
    Node* head;
    Node* tail;
    size_t size;
} List;

static size_t get_elevation(char c)
{
    if (c == 'S') {
        return 0;
    } else if (c == 'E') {
        return 'z' - 'a';
    }
    return c - 'a';
}

static Node* create_node(Point* point)
{
    Node* ret = calloc(1, sizeof(Node));
    ret->point = point;
    return ret;
}

static void destroy_node(Node* node)
{
    free(node);
}

static List* create_list()
{
    List* ret = calloc(1, sizeof(List));
    return ret;
}

static void destroy_list(List* list)
{
    Node* cur_node = list->head;
    Node* temp;
    while (cur_node != NULL) {
        temp = cur_node->next;
        destroy_node(cur_node);
        cur_node = temp;
    }
    free(list);
}

static void append_node(List* list, Node* node)
{
    if (list->size == 0) {
        list->head = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
    }
    list->tail = node;
    node->next = NULL;
    list->size++;
}

static void remove_node(List* list, Node* node)
{
    if (list->size == 0) {
        return;
    }

    Node* temp;
    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else if (node == list->head) {
        list->head = node->next;
        list->head->prev = NULL;
    } else if (node == list->tail) {
        list->tail = node->prev;
        list->tail->next = NULL;
    } else {
        temp = node->prev;
        temp->next = node->next;
        node->next->prev = temp;
    }
    list->size--;

    node->prev = NULL;
    node->next = NULL;
}

static Node* remove_min(List* list)
{
    Node* cur_node = list->head;
    Node* max = list->head;
    Point* cur_point;
    while (cur_node != NULL) {
        cur_point = cur_node->point;
        if (cur_point->distance < max->point->distance) {
            max = cur_node;
        }
        cur_node = cur_node->next;
    }
    remove_node(list, max);
    return max;
}

static void relax_edges(Point* src)
{
    Point* dest;
    for (int i = 0; i < src->num_neighbors; i++) {
        dest = src->neighbors[i];
        if (dest->elevation >= src->elevation - 1 &&
            dest->distance > src->distance + 1) {
                dest->distance = src->distance + 1;
                dest->prev = src;
        }
    }
}

static void initialize_dijkstra_list(List* list)
{
    for (Node* cur_node = list->head; cur_node != NULL; cur_node = cur_node->next) {
        cur_node->point->distance = SIZE_MAX;
    }
}

static void dijkstras(Point* src, List* visited_list, List* unvisited_list)
{
    size_t num_elems = unvisited_list->size;
    // Initialize the list
    initialize_dijkstra_list(unvisited_list);
    src->distance = 0;

    // Dijkstra
    Node* cur_node;
    while (unvisited_list->size > 0) {
        // Get the min from unvisited
        cur_node = remove_min(unvisited_list);
        // Mark it as visited
        // cur_node->point->visited = TRUE;
        append_node(visited_list, cur_node);
        // Relax its edges
        relax_edges(cur_node->point);
    }
}

int main(int argc, char *argv[])
{
    FILE *input = fopen("input.txt", "r");
    if (input == NULL) {
        perror("fopen() error:");
        return EXIT_FAILURE;
    }

    size_t buf_size = 1024;
    char *line = calloc(1, sizeof(char) * buf_size);

    // Get num cols
    getline(&line, &buf_size, input);
    NUM_COLS = strlen(line) - 1; // -1 to exclude newline

    // Get num rows
    NUM_ROWS = 1;
    while (getline(&line, &buf_size, input) != -1 && *line != '\n') {
        NUM_ROWS++;
    }
    rewind(input);

    // Build 2D array out of input
    List* unvisited_points = create_list();
    Point* start;
    Point* end;
    Point* cur_point;
    Point map[NUM_ROWS][NUM_COLS]; // 2D array to hold elevation map
    char raw_map[NUM_ROWS][NUM_COLS];
    for (int row = 0; row < NUM_ROWS; row++) {
        getline(&line, &buf_size, input);
        for (int col = 0; col < NUM_COLS; col++) {
            cur_point = &map[row][col];
            cur_point->row = row;
            cur_point->col = col;
            cur_point->elevation = get_elevation(line[col]);
            cur_point->distance = SIZE_MAX;
            cur_point->prev = NULL;

            raw_map[row][col] = line[col];

            // Get the point's neighbors
            cur_point->num_neighbors = 0;
            for (int i = 0; i < 4; i++) {
                cur_point->neighbors[i] = NULL;
            }
            if (row > 0) {
                cur_point->neighbors[cur_point->num_neighbors++] = &map[row - 1][col];
            }
            if (row < NUM_ROWS - 1) {
                cur_point->neighbors[cur_point->num_neighbors++] = &map[row + 1][col];
            }
            if (col < NUM_COLS - 1) {
                cur_point->neighbors[cur_point->num_neighbors++] = &map[row][col + 1];
            }
            if (col > 0) {
                cur_point->neighbors[cur_point->num_neighbors++] = &map[row][col - 1];
            }

            if (line[col] == 'S') {
                start = cur_point;
            } else if (line[col] == 'E') {
                end = cur_point;
            }

            Node* new_node = create_node(cur_point);
            append_node(unvisited_points, new_node);

            putchar(line[col]);
        }
        putchar('\n');
    }

    free(line);
    fclose(input);

    List* visited_points = calloc(1, sizeof(List));

    /* Dijkstra's to find shortest path from start to end */
    
    /* PART 1 */
    dijkstras(end, visited_points, unvisited_points);
    printf("Length of shortest path from S: %lu\n", start->distance);

    /* PART 2 */
    List* temp;
    size_t min_length = SIZE_MAX;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (raw_map[i][j] != 'a') {
                continue;
            }

            cur_point = &map[i][j];
            min_length = cur_point->distance < min_length ? cur_point->distance : min_length;
        }
    }

    printf("The fewest steps from any elevation 0 point is: %lu\n", min_length);

    destroy_list(visited_points);
    destroy_list(unvisited_points);

    return EXIT_SUCCESS;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MAX_DIR_NAME_SIZE 1024
#define MAX_NUM_CHILDREN 1024
#define MAX_DIR_SIZE 100000 // "Find all of the directories with a total size of at most 100000."
#define DISK_SIZE 70000000 // "The total disk space available to the filesystem is 70000000."
#define UPDATE_SIZE 30000000 // "To run the update, you need unused space of at least 30000000."

const char *CD_CMD = "cd";
size_t USED_SPACE;
size_t AVAIL_SPACE;
size_t REQUIRED_SPACE_FOR_UPDATE;

/* Will use a tree to represent the file system */

typedef struct Node Node;

typedef struct Tree {
    Node *root;
} Tree;

typedef struct Node {
    char *name;
    unsigned long int num_children;
    size_t size;
    Node *parent;
    Node *children[MAX_NUM_CHILDREN];
} Node;

Node *create_node(char *name);
void destroy_node(Node *node);
void add_file_size(Node *node, int file_size);
void destroy_tree(Tree *tree);
void destroy_tree_helper(Node *root);
size_t get_dir_size_sums(Node *node);
size_t get_smallest_dir_to_delete(Node *node, size_t smallest_so_far);

int main(int argc, char *argv[]) {
    // Open file
    FILE *input = fopen("input.txt", "r");
    if (input == NULL) {
        perror("fopen() error:");
        return EXIT_FAILURE;
    }
    // Set buffer for getting lines from input
    size_t buf_size = 1024;
    char *line = calloc(buf_size, sizeof(char));

    /* Build a tree out of the filesystem */

    Tree *tree = calloc(1, sizeof(Tree));

    // Get the initial instruction and initialize root node
    getline(&line, &buf_size, input);
    tree->root = create_node("/");

    // Read lines from input until EoF
    Node *cur_node = tree->root;
    char instruction_buf[3] = { 0 };
    char dir_name_buf[MAX_DIR_NAME_SIZE] = { 0 };
    int file_size = 0;
    while (getline(&line, &buf_size, input) != -1) {
        // Check if line is instruction
        if (line[0] == '$') {
            // Handle the specific instruction
            memset(instruction_buf, 0, 3);
            instruction_buf[0] = line[2];
            instruction_buf[1] = line[3];
            if (strcmp(instruction_buf, CD_CMD) == 0) {
                if (line[5] == '.' && line[6] == '.') {
                    cur_node = cur_node->parent;
                } else {
                    memset(dir_name_buf, 0, MAX_DIR_NAME_SIZE);
                    // Determine name of dir moving into
                    for (int i = 5; line[i] != '\n'; i++) {
                        dir_name_buf[i - 5] = line[i];
                    }
                    // Move into the child dir that matches name
                    for (int i = 0; i < cur_node->num_children; i++) {
                        if (strcmp(dir_name_buf, cur_node->children[i]->name) == 0) {
                            cur_node = cur_node->children[i];
                            break;
                        }
                    }
                }
            }
            // Next iterations will handle result of 'ls' command
        } else if (line[0] == 'd') { // Is a directory
            // Get the directory name and add it to the tree
            memset(dir_name_buf, 0, MAX_DIR_NAME_SIZE);
            for (int i = 4; line[i] != '\n'; i++) {
                dir_name_buf[i - 4] = line[i];
            }
            cur_node->children[cur_node->num_children] = create_node(dir_name_buf);
            cur_node->children[cur_node->num_children]->parent = cur_node;
            cur_node->num_children++;
        } else { // Is a file
            // Get the size of the file and add it to size of node
            file_size = strtol(line, NULL, 10);
            add_file_size(cur_node, file_size);
        }
    }

    /* Get the total size of all directories whose size is less than 100,000 */
    printf("Total size of all dirs whose sizes are <= 100000: %lu\n", get_dir_size_sums(tree->root));

    /* Find the smallest directory that when deleted will decrease */
    USED_SPACE = tree->root->size;
    AVAIL_SPACE = DISK_SIZE - USED_SPACE;
    REQUIRED_SPACE_FOR_UPDATE = UPDATE_SIZE - AVAIL_SPACE;
    printf("Disk space that must be freed: %lu\n", REQUIRED_SPACE_FOR_UPDATE);
    printf("Size of smallest dir that can be deleted to free %lu: %lu\n",
            REQUIRED_SPACE_FOR_UPDATE, get_smallest_dir_to_delete(tree->root, tree->root->size));

    free(line);
    destroy_tree(tree);
    fclose(input);

    return EXIT_SUCCESS;
}

Node *create_node(char *name) {
    // USER MUST SET VALUE OF PARENT THEMSELF (if there is one)
    Node *ret = calloc(1, sizeof(Node));
    ret->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(ret->name, name);
    return ret;
}

void destroy_node(Node *node) {
    free(node->name);
    free(node);
}

void add_file_size(Node *node, int file_size) {
    Node *cur_node = node;
    while (cur_node != NULL) {
        cur_node->size += file_size;
        // File also increases the size of all parent directories
        cur_node = cur_node->parent;
    }
}

void destroy_tree(Tree *tree) {
    destroy_tree_helper(tree->root);
    free(tree);
}

void destroy_tree_helper(Node *root) {
    // RECURSIVE DFS TRAVERSAL
    // Base case: cur_node->num_children = 0
    if (root->num_children == 0 ) {
        destroy_node(root);
        return;
    }
    // Recursive case: i < cur_node->num_children
    for (int i = 0; i < root->num_children; i++) {
        destroy_tree_helper(root->children[i]);
    }
    destroy_node(root);
}

size_t get_dir_size_sums(Node *node) {
    size_t ret = 0;
    
    /* BASE CASE */
    if (node == NULL) {
        return ret;
    }

    /* RECURSIVE CASE */

    if (node->size <= MAX_DIR_SIZE) {
        ret += node->size;
    }
    
    for (int i = 0; i < node->num_children; i++) {
        ret += get_dir_size_sums(node->children[i]);
    }
    return ret;
}

size_t get_smallest_dir_to_delete(Node *node, size_t smallest_so_far) {
    size_t ret = smallest_so_far;
    int freed_space = REQUIRED_SPACE_FOR_UPDATE - node->size;
    if (( node->size < smallest_so_far ) && ( freed_space <= 0 )) {
        ret = node->size;
    }

    for (int i = 0; i < node->num_children; i++) {
        ret = get_smallest_dir_to_delete(node->children[i], ret);
    }
    return ret;
}

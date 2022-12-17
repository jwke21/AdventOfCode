#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum BOOL {
    FALSE,
    TRUE,
} BOOL;

typedef struct Tree {
    BOOL is_visible;
    int height;
} Tree;

int main(int argc, char *argv[]) {
    // Open input file
    FILE *input = fopen("input.txt", "r");
    size_t buf_size = 1024;
    char *line = calloc(buf_size, sizeof(char));

    /* Get the number of rows */

    int num_rows = 0;
    while (getline(&line, &buf_size, input) != -1) {
        if (line[0] != '\n') {
            num_rows++;
        }
    }
    fclose(input);

    /* Build a matrix of Tree structs to represent tree cover */
    
    // Reopen the input
    input = fopen("input.txt", "r");

    // Read first line to get length of a row
    getline(&line, &buf_size, input);

    // Array of arrays will store int values of each tree's height
    int num_cols = strlen(line) - 1; // Don't want to include \n char
    Tree *tree_cover[num_rows][num_cols];

    // Add first row to tree cover matrix
    int cur_row = 0;
    for (int i = 0; i < num_cols; i++) {
        tree_cover[cur_row][i] = calloc(1, sizeof(Tree));
        tree_cover[cur_row][i]->height = line[i] - '0';
        tree_cover[cur_row][i]->is_visible = FALSE;
    }
    cur_row++;

    // Read lines till EoF adding trees to matrix
    while (getline(&line, &buf_size, input) != -1) {
        for (int i = 0; i < num_cols; i++) {
            tree_cover[cur_row][i] = calloc(1, sizeof(Tree));
            tree_cover[cur_row][i]->height = line[i] - '0';
            tree_cover[cur_row][i]->is_visible = FALSE;
        }
        cur_row++;
    }
    fclose(input);

    /* Find visible trees */

    int visible_trees = 0;
    Tree *cur_tree = NULL;
    // "A tree is visible if all of the other trees between it and an edge of the grid are shorter than it."
    for (cur_row = 0; cur_row < num_rows; cur_row++) {
        for (int cur_col = 0; cur_col < num_cols; cur_col++) {
            cur_tree = tree_cover[cur_row][cur_col];
            
            // "All of the trees around the edge of the grid are visible"
            if (cur_row == 0 || cur_row == num_rows - 1 || cur_col == 0 || cur_col == num_cols - 1) {
                cur_tree->is_visible = TRUE;
                visible_trees++;
                continue;
            }
            // Check up
            for (int i = cur_row - 1; i >= 0 && cur_tree->is_visible == FALSE; i--) {
                if (tree_cover[i][cur_col]->height >= cur_tree->height) {
                    break;
                }
                if (i == 0) {
                    cur_tree->is_visible = TRUE;
                    visible_trees++;
                }
            }
            // Check right
            for (int i = cur_col + 1; i < num_cols && cur_tree->is_visible == FALSE; i++) {
                if (tree_cover[cur_row][i]->height >= cur_tree->height) {
                    break;
                }
                if (i == num_cols - 1) {
                    cur_tree->is_visible = TRUE;
                    visible_trees++;
                }
            }
            // Check down
            for (int i = cur_row + 1; i < num_rows && cur_tree->is_visible == FALSE; i++) {
                if (tree_cover[i][cur_col]->height >= cur_tree->height) {
                    break;
                }
                if (i == num_rows - 1) {
                    cur_tree->is_visible = TRUE;
                    visible_trees++;
                }
            }
            // Check left
            for (int i = cur_col - 1; i >= 0 && cur_tree->is_visible == FALSE; i--) {
                if (tree_cover[cur_row][i]->height >= cur_tree->height) {
                    break;
                }
                if (i == 0) {
                    cur_tree->is_visible = TRUE;
                    visible_trees++;
                }
            }
        }
    }

    printf("Visible trees: %d\n", visible_trees);

    /* Get the max scenic score */

    int up = 0, right = 0, down = 0, left = 0;
    int max_score = 0, score = 0;
    // Edges can be excluded because they will not have a scenic score > 0
    for (cur_row = 1; cur_row < num_rows - 1; cur_row++) {
        for (int cur_col = 1; cur_col < num_cols - 1; cur_col++) {
            up = 0, right = 0, down = 0, left = 0;
            cur_tree = tree_cover[cur_row][cur_col];
            // Get up score
            for (int i = cur_row - 1; i >= 0; i--) {
                up++;
                if (tree_cover[i][cur_col]->height >= cur_tree->height) {
                    break;
                }
            }
            // Get right score
            for (int i = cur_col + 1; i < num_cols; i++) {
                right++;
                if (tree_cover[cur_row][i]->height >= cur_tree->height) {
                    break;
                }
            }
            // Get down score
            for (int i = cur_row + 1; i < num_rows; i++) {
                down++;
                if (tree_cover[i][cur_col]->height >= cur_tree->height) {
                    break;
                }
            }
            // Get left score
            for (int i = cur_col - 1; i >= 0; i--) {
                left++;
                if (tree_cover[cur_row][i]->height >= cur_tree->height) {
                    break;
                }
            }
            score = up * right * down * left;
            if (score > max_score) {
                max_score = score;
            }
        }
    }

    printf("Max scenic score: %d\n", max_score);

    // Destroy the matrix
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < num_rows; j++) {
            free(tree_cover[i][j]);
        }
    }
    free(line);

    return EXIT_SUCCESS;
}
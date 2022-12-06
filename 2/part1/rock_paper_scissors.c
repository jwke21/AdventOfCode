#include <stdlib.h>
#include <stdio.h>

#define ROCK_PTS 1
#define PAPER_PTS 2
#define SCISSORS_PTS 3
#define LOSE_PTS 0
#define DRAW_PTS 3
#define WIN_PTS 6

#define MY_ROCK 'X'
#define MY_PAPER 'Y'
#define MY_SCISSORS 'Z'
#define OP_ROCK 'A'
#define OP_PAPER 'B'
#define OP_SCISSORS 'C'


int get_round_score(char op_move, char my_move);

int main(int argc, char *argv[]) {
    size_t buf_size = 1024;
    char *line = NULL;
    FILE *input = NULL;
    int total_score = 0;

    // Open file
    input = fopen("../input", "r");

    // Get the lines until EoF
    while (getline(&line, &buf_size, input) != -1) {
        if (line[0] == '\n') {
            continue;
        }
        // Get round score
        total_score += get_round_score(line[0], line[2]); 
    }

    free(line);
    fclose(input);

    printf("Total score is: %d\n", total_score);

    return 0;
} 

int get_round_score(char op_move, char my_move) {
    int shp_score = 0;
    int res_score = 0;

    // Determine the score from move type and result of round
    switch (my_move) {
        // Rock
        case MY_ROCK:
            shp_score = ROCK_PTS;
            switch (op_move) {
                case OP_ROCK:
                    res_score = DRAW_PTS;
                    break;
                case OP_PAPER:
                    res_score = LOSE_PTS;
                    break;
                case OP_SCISSORS:
                    res_score = WIN_PTS;
                    break;
            }
            break;
        // Paper
        case MY_PAPER:
            shp_score = PAPER_PTS;
            switch (op_move) {
                case OP_ROCK:
                    res_score = WIN_PTS;
                    break;
                case OP_PAPER:
                    res_score = DRAW_PTS;
                    break;
                case OP_SCISSORS:
                    res_score = LOSE_PTS;
                    break;
            }
            break;
        // Scissors
        case MY_SCISSORS:
            shp_score = SCISSORS_PTS;
            switch (op_move) {
                case OP_ROCK:
                    res_score = LOSE_PTS;
                    break;
                case OP_PAPER:
                    res_score = WIN_PTS;
                    break;
                case OP_SCISSORS:
                    res_score = DRAW_PTS;
                    break;
            }
            break;
    }

    return shp_score + res_score;
}

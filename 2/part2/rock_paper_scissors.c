#include <stdio.h>

#define ROCK_PTS 1
#define PAPER_PTS 2
#define SCISSORS_PTS 3
#define LOSE_PTS 0
#define DRAW_PTS 3
#define WIN_PTS 6

#define LOSE 'X'
#define DRAW 'Y'
#define WIN 'Z'
#define ROCK 'A'
#define PAPER 'B'
#define SCISSORS 'C'


int get_round_score(char op_move, char res);

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

    printf("Total score is: %d\n", total_score);

    return 0;
} 

int get_round_score(char op_move, char res) {
    int shp_score = 0;
    int res_score = 0;

    // Determine the score from move type and result of round
    switch (op_move) {
        // Opponent plays rock
        case ROCK:
            shp_score = ROCK_PTS;
            switch (res) {
                case LOSE:
                    shp_score = SCISSORS_PTS;
                    res_score = LOSE_PTS;
                    break;
                case DRAW:
                    shp_score = ROCK_PTS;
                    res_score = DRAW_PTS;
                    break;
                case WIN:
                    shp_score = PAPER_PTS;
                    res_score = WIN_PTS;
                    break;
            }
            break;
        // Opponent plays paper
        case PAPER:
            switch (res) {
                case LOSE:
                    shp_score = ROCK_PTS;
                    res_score = LOSE_PTS;
                    break;
                case DRAW:
                    shp_score = PAPER_PTS;
                    res_score = DRAW_PTS;
                    break;
                case WIN:
                    shp_score = SCISSORS_PTS;
                    res_score = WIN_PTS;
                    break;
            }
            break;
        // Opponenent plays scissors
        case SCISSORS:
            switch (res) {
                case LOSE:
                    shp_score = PAPER_PTS;
                    res_score = LOSE_PTS;
                    break;
                case DRAW:
                    shp_score = SCISSORS_PTS;
                    res_score = DRAW_PTS;
                    break;
                case WIN:
                    shp_score = ROCK_PTS;
                    res_score = WIN_PTS;
                    break;
            }
            break;
    }

    return shp_score + res_score;
}

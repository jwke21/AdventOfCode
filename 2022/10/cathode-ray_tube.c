#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int LAST_CYCLE = 240;
const char *NOOP_INSTR = "noop";
const char *ADDX_INSTR = "addx";
const int CRT_WIDTH = 40;
const int CRT_HEIGHT = 6;

typedef enum BOOL {
    FALSE,
    TRUE,
} BOOL;

static void
display_crt_screen(char screen[CRT_HEIGHT][CRT_WIDTH]) {
    for (int i = 0; i < CRT_HEIGHT; i++) {
        for (int j = 0; j < CRT_WIDTH; j++) {
            putchar(screen[i][j]);
        }
        putchar('\n');
    }
}

int main(int argc, char *argv[]) {
    FILE *input = fopen("input.txt", "r");
    if (input == NULL) {
        perror("fopen() error:");
        return EXIT_FAILURE;
    }

    size_t buf_size = 16;
    char *line = calloc(buf_size, sizeof(char));

    int x_reg = 1; // The X register value
    char screen[CRT_HEIGHT][CRT_WIDTH];
    for (int i = 0; i < CRT_HEIGHT; i++) {
        for (int j = 0; j < CRT_WIDTH; j++) {
            screen[i][j] = '.';
        }
    }

    char instruction_buf[5] = { 0 };
    int add_value = 0;
    int sig_strength = 0;
    int cur_crt_row = -1;
    int draw_pos = 0;
    BOOL add_instruct = FALSE;
    for (int cycle = 1; cycle <= LAST_CYCLE; cycle++) {
        if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220) {
            sig_strength += cycle * x_reg;
        }

        // Update the CRT screen
        if ((cycle - 1) % 40 == 0) {
            cur_crt_row++;
        }
        draw_pos = (cycle - 1) % 40;
        // Draw a pixel if the sprite is within draw position
        if (draw_pos == x_reg || draw_pos == x_reg - 1 || draw_pos == x_reg + 1) {
            screen[cur_crt_row][draw_pos] = '#';
        }

        // Get next instruction to exec if necessary
        if (instruction_buf[0] == 0) {
            getline(&line, &buf_size, input);
            strncpy(instruction_buf, line, 4); // Copies instruction into buf
        }
        // Handle if the instruction is addx
        if (strcmp(ADDX_INSTR, instruction_buf) == 0) {
            if (add_instruct == TRUE) { // Second cycle of addx -> carryout operation
                x_reg += add_value;
                add_instruct = FALSE;
                memset(instruction_buf, 0, 5);
            } else {                    // First cycle of addx -> get V
                add_value = strtol(&line[4], NULL, 10);
                add_instruct = TRUE;
            }
        } else {
            // If the instruction is noop, do nothing for cycle
            memset(instruction_buf, 0, 5);
        }
    }

    printf("Signal strength sum after 220 cycles: %d\n", sig_strength);

    display_crt_screen(screen);

    free(line);
    fclose(input);

    return EXIT_SUCCESS;
}
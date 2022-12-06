#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char *argv[]) {
    size_t buf_size = 1024;
    char *line = NULL;
    char *mut_line = NULL;
    long int numbers[4] = { 0 };
    int i;
    int contains = 0;
    int overlaps = 0;

    // Open the file
    FILE *input = fopen("input", "r");
    if (input == NULL) {
        perror("fopen() failure");
        return EXIT_FAILURE;
    }

    // Read line by line until EoF
    while (getline(&line, &buf_size, input) != -1) {
        // Get all numbers from line
        i = 0;
        mut_line = line;
        while (*mut_line != '\n' && i < 4) {
            // Check if the char is the beginning of a number
            if (isdigit(*mut_line) != 0) {
                numbers[i] = strtol(mut_line, &mut_line, 10);
                i++;
            } else {
                mut_line++;
            }
        }

        // Check if either of the elfs' ranges completely contains the other's
        if ((numbers[0] < numbers[2] && numbers[1] >= numbers[3]) || // elf 1 contains elf 2
            (numbers[2] < numbers[0] && numbers[3] >= numbers[1]) || // elf 2 contians elf 1
            numbers[0] == numbers[2])
        {
            contains++;
        }
        // Check if either elf's range overlaps with the others
        if ((numbers[0] < numbers[2] && numbers[1] >= numbers[2]) ||
            (numbers[2] < numbers[0] && numbers[3] >= numbers[0]) ||
            numbers[0] == numbers[2])
        {
            overlaps++;
        }
    }

    fclose(input);

    printf("Number of pairs where one contains the other: %d\n", contains);
    printf("Number of pairs with overlapping ranges: %d\n", overlaps);

    return EXIT_SUCCESS;
} 
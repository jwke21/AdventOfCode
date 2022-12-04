#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    int max;
    int elf_carried_calories;
    size_t buf_size = 1024;
    char *line = NULL;
    FILE *input;
    int top_three_total[3] = {0, 0, 0};
    int temp;

    // Open the input file in read only mode
    input = fopen("./input", "r");
    if (input == NULL) {
        return -1;
    }

    max = elf_carried_calories = 0;
    // Read line by line until end of file
    while (getline(&line, &buf_size, input) != -1) {
        // If all the calories carried by current elf have been counted
        if (strcmp(line, "\n") == 0) {
            // Compare the max to the amount carried by current elf
            if (elf_carried_calories > max) {
                max = elf_carried_calories;
            }

            // Compare with top three elves
            for (int i = 0; i < 3; i++) {
                // If it is larger than one of the top three, swap that value with elf_carried_calories
                if (elf_carried_calories > top_three_total[i]) {
                    temp = top_three_total[i];
                    top_three_total[i] = elf_carried_calories;
                    elf_carried_calories = temp;
                }
            }

            // Prepare for next elf
            elf_carried_calories = 0;
        }
        else {
            // Add the current line's calories to total carried by elf
            elf_carried_calories += atoi(line);
        }
    }

    // Close the input file
    fclose(input);

    // Print the max
    printf("%d\n", max);
    // Print top three total
    printf("%d\n", top_three_total[0] + top_three_total[1] + top_three_total[2]);

    return 0;
}
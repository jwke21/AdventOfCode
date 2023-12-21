#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


int get_priority(char c);

int main(int argc, char *argv[]) {
    int priorities[52];
    FILE *input = NULL;
    size_t buf_size = 1024;
    char *line = NULL;
    int line_size = 0;
    int priority = 0;
    int total = 0;
    int group_members_seen = 0;
    int group_member_priorities[3][52];
    int badge_totals = 0;

    // Open the input file
    input = fopen("./input", "r");
    if (input == NULL) {
        perror("Failed to open file\n");
        return 1;
    }

    // Initialize the arrays storing each group members' priorities
    for (int i = 0; i < 3; i++) {
        bzero(group_member_priorities[i], 52 * sizeof(int));
    }

    // Read lines till EoF
    while (getline(&line, &buf_size, input) != -1) {
        // Count the group member as seen
        group_members_seen++;
    
        // Get length of line 
        line_size = strlen(line);
        // Zero out priorities array
        bzero(priorities, 52 * sizeof(int));
        // Get priorities that appear in left half of line
        for (int i = 0; i < (line_size / 2); i++) {
            priority = get_priority(line[i]);
            priorities[priority] += 1;
        }
        // Check each priority in the second half
        for (int i = (line_size / 2); i < line_size; i++) {
            priority = get_priority(line[i]);
            // Once a priority duplicate has been found add it to total
            if (priorities[priority] > 0) {
                total += priority + 1;  // + 1 because the priorities start at 1
                break;
            }
        }
        // Loop over second half again to add then to the line's overall priorities
        for (int i = (line_size / 2); (i < line_size) && (line[i] != '\n'); i++) {
            priority = get_priority(line[i]);
            priorities[priority] += 1;
        }

        // Copy the group member's priorities into the their array for the group
        memcpy(group_member_priorities[group_members_seen - 1], priorities, 52 * sizeof(int));
        // Check if all the group members have been seen
        if (group_members_seen == 3) {
            // Find their common badge and add it to the badge total
            for (int i = 0; i < 52; i++) {
                if (group_member_priorities[0][i] > 0 &&
                    group_member_priorities[1][i] > 0 &&
                    group_member_priorities[2][i] > 0) {
                    badge_totals += i + 1;
                    break;
                }
            }
            // Re-initialize array of group members' priorities
            for (int i = 0; i < 3; i++) {
                bzero(group_member_priorities[i], 52 * sizeof(int));
            }
            group_members_seen = 0;
        }
    }

    free(line);
    fclose(input);

    printf("Common compartment total is: %d\n", total);
    printf("Common badges total is: %d\n", badge_totals);

    return 0;
}

int get_priority(char c) {
    int priority = 0;

    switch (c) {
        case 'a':
            priority = 0;
            break;
        case 'b':
            priority = 1;
            break;
        case 'c':
            priority = 2;
            break;
        case 'd':
            priority = 3;
            break;
        case 'e':
            priority = 4;
            break;
        case 'f':
            priority = 5;
            break;
        case 'g':
            priority = 6;
            break;
        case 'h':
            priority = 7;
            break;
        case 'i':
            priority = 8;
            break;
        case 'j':
            priority = 9;
            break;
        case 'k':
            priority = 10;
            break;
        case 'l':
            priority = 11;
            break;
        case 'm':
            priority = 12;
            break;
        case 'n':
            priority = 13;
            break;
        case 'o':
            priority = 14;
            break;
        case 'p':
            priority = 15;
            break;
        case 'q':
            priority = 16;
            break;
        case 'r':
            priority = 17;
            break;
        case 's':
            priority = 18;
            break;
        case 't':
            priority = 19;
            break;
        case 'u':
            priority = 20;
            break;
        case 'v':
            priority = 21;
            break;
        case 'w':
            priority = 22;
            break;
        case 'x':
            priority = 23;
            break;
        case 'y':
            priority = 24;
            break;
        case 'z':
            priority = 25;
            break;
        case 'A':
            priority = 26;
            break;
        case 'B':
            priority = 27;
            break;
        case 'C':
            priority = 28;
            break;
        case 'D':
            priority = 29;
            break;
        case 'E':
            priority = 30;
            break;
        case 'F':
            priority = 31;
            break;
        case 'G':
            priority = 32;
            break;
        case 'H':
            priority = 33;
            break;
        case 'I':
            priority = 34;
            break;
        case 'J':
            priority = 35;
            break;
        case 'K':
            priority = 36;
            break;
        case 'L':
            priority = 37;
            break;
        case 'M':
            priority = 38;
            break;
        case 'N':
            priority = 39;
            break;
        case 'O':
            priority = 40;
            break;
        case 'P':
            priority = 41;
            break;
        case 'Q':
            priority = 42;
            break;
        case 'R':
            priority = 43;
            break;
        case 'S':
            priority = 44;
            break;
        case 'T':
            priority = 45;
            break;
        case 'U':
            priority = 46;
            break;
        case 'V':
            priority = 47;
            break;
        case 'W':
            priority = 48;
            break;
        case 'X':
            priority = 49;
            break;
        case 'Y':
            priority = 50;
            break;
        case 'Z':
            priority = 51;
            break;
    }

    return priority;
}

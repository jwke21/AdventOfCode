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
        for (int i = (line_size / 2); i < line_size; i++) {
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

    fclose(input);

    printf("Common compartment total is: %d\n", total);
    printf("Common badges total is: %d\n", badge_totals);

    return 0;
}

int get_priority(char c) {
    int priority = 0;

    // If the char is a lowercase letter
    if (isupper(c) == 0) {
        priority = c - 'a';
    } else {
        priority = c - 'A' + 26;
    }

    return priority;
}

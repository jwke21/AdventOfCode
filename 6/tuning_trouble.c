#include <stdlib.h>
#include <stdio.h>

typedef enum BOOL {
    FALSE,
    TRUE,
} BOOL;

void place_in_window(int *window_chars, char c);
void remove_from_window(int *window_chars, char c);
BOOL more_than_one_in_window(int *window_chars, char c);

int main(int argc, char *argv[]) {
    size_t MAX_BUF_SIZE = 4097;
    char *line = NULL;

    // Open file
    FILE *input = fopen("input", "r");

    if (input == NULL) {
        perror("fopen() error:");
        return EXIT_FAILURE;
    }

    // Read file into buffer
    getline(&line, &MAX_BUF_SIZE, input);

    /* Sliding window to find markers */

    int right = 3, left = 0;
    int window_chars[26] = { 0 }; // chars in input sig are lowercase letters
    // Initialize starting window
    for (int i = left; i <= right; i++) {
        place_in_window(window_chars, line[i]);
    }

    // Find markers
    printf("beginning search for start-of-packet marker...\n");
    char cur_char;
    int i;
    BOOL start_of_pkt_found = FALSE;
    while (right < MAX_BUF_SIZE) {
        // PART 1: find start of packet
        if (start_of_pkt_found == FALSE) {
            // Check if window is first marker
            for (i = left; i <= right; i++) {
                cur_char = line[i];
                if (more_than_one_in_window(window_chars, cur_char) == TRUE) {
                    place_in_window(window_chars, line[++right]);
                    remove_from_window(window_chars, line[left++]);
                    break;
                }
            }
            // Every window element is unique so window is first marker
            if (i > right) {
                printf("first marker after character: %d\n", right + 1);
                // Set the vars to begin looking for start of first message
                right = left + 13; // Start of message marker is 14 chars
                // Initialize the new window size (14 chars)
                for (i = left + 4; i <= right; i++) {
                    place_in_window(window_chars, line[i]);
                }
                printf("beginning search for first start-of-message marker...\n");
                start_of_pkt_found = TRUE;
            }
        } else { // PART 2: Finding start-of-message marker
            // Check if window is start-of-message marker
            for (i = left; i <= right; i++) {
                cur_char = line[i];
                if (more_than_one_in_window(window_chars, cur_char) == TRUE) {
                    place_in_window(window_chars, line[++right]);
                    remove_from_window(window_chars, line[left++]);
                    break;
                }
            }
            // Every window element is unique so window is start-of-message
            if (i > right) {
                printf("start-of-message after character: %d\n", right + 1);
                break;
            }
        }
    }

    free(line);
    fclose(input);
    
    return EXIT_SUCCESS;
}

void place_in_window(int *window_chars, char c) {
    window_chars[c - 'a'] += 1;
}

void remove_from_window(int *window_chars, char c) {
    window_chars[c - 'a'] -= 1;
}

BOOL more_than_one_in_window(int *window_chars, char c) {
    if (window_chars[c - 'a'] > 1) {
        return TRUE;
    }
    return FALSE;
}


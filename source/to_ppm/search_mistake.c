#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* my_file = fopen("int.txt", "r");
    if (my_file == NULL) {
        perror("Error opening int.txt");
        return 1;
    }

    FILE* correct_file = fopen("correct.txt", "r");
    if (correct_file == NULL) {
        perror("Error opening correct.txt");
        fclose(my_file);
        return 1;
    }

    int my_char, correct_char;
    int position = 1;
    int error_count = 0;

    while ((my_char = fgetc(my_file)) != EOF && (correct_char = fgetc(correct_file)) != EOF) {
        if (my_char != correct_char) {
            printf("Mismatch at position %d: My Value = %c, Correct Value = %c\n", position, my_char, correct_char);
            error_count++;
        }
        position++;
    }

    // ファイルのいずれかがまだ読み取れる場合、長さが一致しないことを示す
    if (my_char != EOF || correct_char != EOF) {
        fprintf(stderr, "Warning: The files have different lengths.\n");
    }

    if (error_count == 0) {
        printf("All characters match.\n");
    } else {
        printf("Total mismatches: %d\n", error_count);
    }

    fclose(my_file);
    fclose(correct_file);

    return 0;
}
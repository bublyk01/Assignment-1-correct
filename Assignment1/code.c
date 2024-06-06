#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLEAR_COMMAND "cls"

int text_input();
int save_text(const char* filename);
int load_text(const char* filename);
char getCharAt(char** lines, int lineNumber, int index);
void insertTextAt(char** lines, int lineNumber, int index, const char* text);
int searchText(char** lines, int lineCount, const char* searchText, int positions[][2]);
void clearConsole();

char** text_lines = NULL;
int line_count = 0;

int main() {
    char command[10];

    while (1) {
        clearConsole();
        printf("Choose the command: ");
        scanf_s("%s", command, (unsigned)_countof(command));
        getchar();

        if (strcmp(command, "1") == 0) {
            text_input();
        }
        else if (strcmp(command, "2") == 0) {
            printf("Enter the file name for saving: ");
            char filename[100];
            scanf_s("%s", filename, (unsigned)_countof(filename));
            save_text(filename);
            printf("Text has been saved successfully\n");
        }
        else if (strcmp(command, "3") == 0) {
            printf("Enter the file name for loading: ");
            char filename[100];
            scanf_s("%s", filename, (unsigned)_countof(filename));
            load_text(filename);
            printf("Text has been loaded successfully\n");
        }
        else if (strcmp(command, "4") == 0) {
            printf("You wrote:\n");
            for (int i = 0; i < line_count; i++) {
                printf("%s\n", text_lines[i]);
            }
        }
        else if (strcmp(command, "5") == 0) {
            int lineNumber, index;
            printf("Enter the line number along with index: ");
            scanf_s("%d %d", &lineNumber, &index);
            getchar();

            printf("Enter text to insert: ");
            char text[100];
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0;

            insertTextAt(text_lines, lineNumber - 1, index, text);
        }
        else if (strcmp(command, "6") == 0) {
            printf("Enter text to search: ");
            char search_text[100];
            fgets(search_text, sizeof(search_text), stdin);
            search_text[strcspn(search_text, "\n")] = 0;

            int positions[100][2];
            int count = searchText(text_lines, line_count, search_text, positions);
            if (count == 0) {
                printf("Text not found\n");
            }
            else {
                for (int i = 0; i < count; i++) {
                    printf("Text was found in this position: %d %d\n", positions[i][0] + 1, positions[i][1] + 1);
                }
            }
        }
        else if (strcmp(command, "help") == 0) {
            printf("1 - text typewriter, 2 - save the file, 3 - load the file, 4 - show what you wrote, 5 - insert text at position, 6 - search\n");
        }
        else if (strcmp(command, "exit") == 0) {
            printf("Exiting the program...\n");
            break;
        }
        else {
            printf("Command was not found\n");
        }

        printf("Press Enter to continue...");
        getchar();
    }

    for (int i = 0; i < line_count; i++) {
        free(text_lines[i]);
    }
    free(text_lines);

    return 0;
}

void clearConsole() {
    system(CLEAR_COMMAND);
}

int text_input() {
    char text[100];
    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;

    text_lines = realloc(text_lines, (line_count + 1) * sizeof(char*));
    text_lines[line_count] = malloc((strlen(text) + 1) * sizeof(char));
    strcpy_s(text_lines[line_count], strlen(text) + 1, text);
    line_count++;
    return 0;
}

int save_text(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < line_count; i++) {
            fprintf(file, "%s\n", text_lines[i]);
        }
        fclose(file);
    }
    else {
        printf("Could not create the file\n");
    }
    return 0;
}

int load_text(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        char line[100];
        for (int i = 0; i < line_count; i++) {
            free(text_lines[i]);
        }
        free(text_lines);
        text_lines = NULL;
        line_count = 0;

        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;

            text_lines = realloc(text_lines, (line_count + 1) * sizeof(char*));
            text_lines[line_count] = malloc((strlen(line) + 1) * sizeof(char));
            strcpy_s(text_lines[line_count], strlen(line) + 1, line);
            line_count++;
        }
        fclose(file);

        printf("Loaded text:\n");
        for (int i = 0; i < line_count; i++) {
            printf("%s\n", text_lines[i]);
        }
    }
    else {
        printf("Could not open the file\n");
    }
    return 0;
}

char getCharAt(char** lines, int lineNumber, int index) {
    if (lineNumber < 0 || lineNumber >= line_count) {
        fprintf(stderr, "Line number out of range\n");
        return '\0';
    }

    char* line = lines[lineNumber];
    if (index < 0 || index >= (int)strlen(line)) {
        fprintf(stderr, "Index out of range\n");
        return '\0';
    }

    return line[index];
}

void insertTextAt(char** lines, int lineNumber, int index, const char* text) {
    if (lineNumber < 0 || lineNumber >= line_count) {
        fprintf(stderr, "Line number out of range\n");
        return;
    }

    char* line = lines[lineNumber];
    if (index < 0 || index >(int)strlen(line)) {
        fprintf(stderr, "Index out of range\n");
        return;
    }

    char* new_line = malloc(strlen(line) + strlen(text) + 1);
    strncpy_s(new_line, strlen(line) + 1, line, index);
    new_line[index] = '\0';
    strcat_s(new_line, strlen(new_line) + strlen(text) + 1, text);
    strcat_s(new_line, strlen(new_line) + strlen(line + index) + 1, line + index);

    free(lines[lineNumber]);
    lines[lineNumber] = new_line;
}

int searchText(char** lines, int lineCount, const char* searchText, int positions[][2]) {
    int count = 0;
    for (int i = 0; i < lineCount; i++) {
        char* pos = strstr(lines[i], searchText);
        while (pos) {
            positions[count][0] = i;
            positions[count][1] = (int)(pos - lines[i]);
            count++;
            pos = strstr(pos + 1, searchText);
        }
    }
    return count;
}

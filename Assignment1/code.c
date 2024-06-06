#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLEAR_COMMAND "cls"

int text_input();
int save_text(const char *filename);
int load_text(const char *filename);
char getCharAt(char **lines, int lineNumber, int index);
void insertTextAt(char **lines, int lineNumber, int index, const char *text);
int searchText(char** lines, int lineCount, const char* searchText, int positions[][2]);
void clearConsole();

char** text_lines = NULL;
int line_count = 0;

int main() {
    char command;

    while (1) {
        clearConsole();
        printf("Choose the command (or type 'exit' to quit): ");
        scanf_s("%s", command, (unsigned)_countof(command));
        getchar();

        if (command, "1") {
            text_input();
        }
        else if (command, "2") {
            printf("Enter the file name for saving: ");
            char filename;
            scanf_s("%s", filename, (unsigned)_countof(filename));
            save_text(filename);
            printf("Text has been saved successfully");
        }
        else if (command, "3") {
            printf("Enter the file name for loading: ");
            char filename;
            scanf_s("%s", filename, (unsigned)_countof(filename));
            load_text(filename);
            printf("Text has been loaded successfully");
        }
        else if (command, "4") {
            printf("You wrote:\n");
            for (int i = 0; i < line_count; i++) {
                printf("%s\n", text_lines[i]);
            }
        }
        else if (command, "5") {
            int lineNumber, index;
            printf("Enter line number and index: ");
            scanf_s("%d %d, &lineNumber, &index");
            getchar();

            printf("Enter text to insert: ");
            char text;
            fgets(text, sizeof(text), stdin);

            insertTextAt(text_lines, lineNumber - 1, index, text);
        }
        else if (command, "6") {
            printf("Enter text to search: ");
            char search_text;
            fgets(search_text, sizeof(search_text), stdin);

            int positions;
            int count = searchText(text_lines, line_count, search_text, positions);
            if (count=0) {
                printf("Text not found");
            }
            else {
                for (int i = 0; i < count; i++) {
                    printf("Text was found in this position: %d %d\n", positions[i][0] + 1, positions[i][1] + 1);
                }
            }
        }
        else if (command, "help") {
            printf("1 - text typewriter, 2 - new line, 3 - save the file, 4 - load the file, 5 - show what you wrote, 6 - insert text at position, 7 - search\n");
        }
        else if (command, "exit") {
            printf("Exiting the program...\n");
            break;
        }
        else {
            printf("Invalid command\n");
        }

        printf("Press Enter to continue...");
        getchar();
    }

    return 0;
}

void clearConsole() {
    system(CLEAR_COMMAND);
}

int text_input() {
    std::string text;
    std::cout << "Enter text: ";
    std::getline(std::cin, text);
    text_lines.push_back(text);
    return 0;
}

int save_text(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& line : text_lines) {
            file << line << "\n";
        }
        file.close();
    }
    else {
        std::cout << "Could not create the file\n";
    }
    return 0;
}

int load_text(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        text_lines.clear();
        while (std::getline(file, line)) {
            text_lines.push_back(line);
        }
        file.close();
        std::cout << "Loaded text:\n";
        for (const auto& line : text_lines) {
            std::cout << line << "\n";
        }
    }
    else {
        std::cout << "Could not open the file\n";
    }
    return 0;
}

char getCharAt(const std::vector<std::string>& lines, int lineNumber, int index) {
    if (lineNumber < 0 || lineNumber >= lines.size()) {
        std::cerr << "Line number out of range" << std::endl;
        return '\0';
    }

    const std::string& line = lines[lineNumber];
    if (index < 0 || index >= line.size()) {
        std::cerr << "Index out of range" << std::endl;
        return '\0';
    }

    return line[index];
}

void insertTextAt(std::vector<std::string>& lines, int lineNumber, int index, const std::string& text) {
    if (lineNumber < 0 || lineNumber >= lines.size()) {
        std::cerr << "Line number out of range" << std::endl;
        return;
    }

    std::string& line = lines[lineNumber];
    if (index < 0 || index > line.size()) {
        std::cerr << "Index out of range" << std::endl;
        return;
    }

    line.insert(index, text);
}

std::vector<std::pair<int, int>> searchText(const std::vector<std::string>& lines, const std::string& searchText) {
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < lines.size(); ++i) {
        size_t pos = lines[i].find(searchText);
        while (pos != std::string::npos) {
            positions.emplace_back(i, pos);
            pos = lines[i].find(searchText, pos + 1);
        }
    }
    return positions;
}

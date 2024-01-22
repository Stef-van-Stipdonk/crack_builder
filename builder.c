#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
} Command;

int append(char **str, const char *new_content) {
    if (*str == NULL) {
        *str = (char *)malloc(strlen(new_content) + 1);
    } else {
        *str = (char *)realloc(*str, strlen(*str) + strlen(new_content) + 1);
    }

    if (*str == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
        return -1;
    }
    strcat(*str, " ");
    strcat(*str, new_content);

    return 1;
}

int add_flag(Command *cmd, const char *flags) {
    return append(&(cmd->str), flags);
}

int add_file(Command *cmd, const char *file_path) {
    return append(&(cmd->str), file_path);
}

int add_directory(Command *cmd, const char *directory_path) {
    return append(&(cmd->str), directory_path);
}

int main(void) {
    Command cmd;
    cmd.str = NULL; // Initialize str to NULL
    add_flag(&cmd, "gcc");
    add_flag(&cmd, "-g");
    add_flag(&cmd, "-o test");
    
    // add_flag(&cmd, "-Wall");
    add_file(&cmd, "builder.c");
    // add_directory(&cmd, ".");

    printf("%s", cmd.str);
    system(cmd.str);
    // Free allocated memory
    free(cmd.str);

    return 0;
}

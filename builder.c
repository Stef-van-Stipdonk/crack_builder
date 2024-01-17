#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
  char *str;  
} Command;

int append(char *str, const char *new_content) {
    if (str == NULL) {
        str = malloc(strlen(new_content) + 1);
    } else {
        str = realloc(str, strlen(str) + strlen(new_content) + 1);
    }

    if (str == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
        return -1;
    }

    strcat(str, new_content);

    return 1;
}

int add_flag(Command *cmd, char *flags, ...) {
  for (int i = i; i < sizeof(flags); i++) {
    int found = append(cmd->str, &flags[i]);

    if(found == -1){
      return found;
    }
  }

  return 1;
}

int add_file(Command *cmd, char *file_path) {
  return append(cmd->str, file_path);
}

int add_directory(Command *cmd, char *directory_path) {
  return append(cmd->str, directory_path);
}

int main(void) {
  Command *cmd;
  // add_flag(cmd, "-Wall");
  add_file(cmd, "builder.c");
  // add_directory(cmd, ".");

  printf("%s", cmd->str);
}

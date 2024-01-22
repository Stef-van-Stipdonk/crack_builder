#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
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

int add_flag(Command *cmd, int count, ...) {
    va_list args;
    va_start(args, count);

    for(int i = 0; i < count; i++) {
        append(&(cmd->str), va_arg(args, char *));
    }

    va_end(args);

    return 1;
}

int add_file(Command *cmd, const char *file_path) {
    return append(&(cmd->str), file_path);
}

int add_directory(Command *cmd, char *directory_path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(directory_path);
    if(d){
        while((dir = readdir(d)) != NULL) {
            struct stat path_stat;
            printf("%s", strcat(dir->d_name, "\n"));
            if(stat(dir->d_name, &path_stat) == 0 && S_ISDIR(path_stat.st_mode)){
                char *new_path = (char *)malloc(strlen(directory_path) + strlen(dir->d_name) + 1);
                strcpy(new_path, directory_path);
                strcat(new_path, dir->d_name);
                printf("%s", strcat(new_path, "\n"));
                add_directory(cmd, new_path);
                free(new_path);
            }
            else {
                append(&(cmd->str), dir->d_name);
            }
        }
        closedir(d);
        free(dir);
    }

    return 1;
}

void print_file_tree(char *basePath, const int root)
{
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            for (i=0; i<root; i++) 
            {
                if (i%2 == 0 || i == 0)
                    printf("%c", '|');
                else
                    printf("%c", ' ');

            }

            printf("%c%c%s\n", '|', '-', dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            print_file_tree(path, root + 2);
        }
    }

    closedir(dir);
}

int main(void) {
    Command cmd;
    cmd.str = NULL;
    add_flag(&cmd, 1, "gcc");
    add_flag(&cmd, 1, "-g");
    add_flag(&cmd, 2, "-o", "test");
    
    add_file(&cmd, "builder.c");
    add_file(&cmd, "test.c");

//    add_directory(&cmd, "./");
    print_file_tree(".", 0);
    // system(cmd.str);

    free(cmd.str);

    return 0;
}

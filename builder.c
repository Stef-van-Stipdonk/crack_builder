#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

typedef struct {
    List(char*) command_list;
} Command;

void get_config_file(char *path, char* ignore_list[], size_t size);

void add_flag(Command *cmd, int count, ...) {
    va_list args;
    va_start(args, count);

    for(int i = 0; i < count; i++) {
        listAppend(cmd->command_list, va_arg(args, char *));
    }

    va_end(args);
}

void add_file(Command *cmd, char *file_path) {
    listAppend(cmd->command_list, file_path);
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
            for (i = 0; i < root; i++) 
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

int endsWith(char *str, char *end){
    size_t slen = strlen(str);
    size_t elen = strlen(end);

    if(slen < elen)
        return 0;

    return (strcmp(&(str[slen-elen]), end) == 0);
}

void add_directory_impl(Command *cmd, char *basePath, char *ignore_list[], size_t size){
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    bool is_ignored = false;
    while ((dp = readdir(dir)) != NULL)
    {
        for (int i = 0; i < size - 1; ++i) {
            if(ignore_list[i] == NULL) {
                break;
            }

            if(strcmp(dp->d_name, ignore_list[i]) == 0){
                printf("%s%s%s\n", "Found ", ignore_list[i], " ignoring.");
                is_ignored = true;
                break;
            }  
        }

        if(is_ignored){
            is_ignored = false;
            continue;
        }
        
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            add_directory_impl(cmd, path, ignore_list, size);
        }

        if(endsWith(dp->d_name, ".c")){
            listAppend(cmd->command_list, path);
        }

    }

    closedir(dir);
}

void add_directory(Command *cmd, char *basePath) {
    size_t size = 1000;
    char *ignore_list[size];
    get_config_file("./ignore.conf", ignore_list, size);
    add_directory_impl(cmd, basePath, ignore_list, size);
}

void get_config_file(char *path, char *ignore_list[], size_t size){
    FILE * fp;
    fp = fopen(path, "r");
    if(fp == NULL)
        exit(EXIT_FAILURE);
    char *line;
    size_t len = 0;
    size_t found_ignores = 0;
    ssize_t read;
    while((read = getline(&line, &len, fp)) != -1){
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        ignore_list[found_ignores] = (char *)malloc(strlen(line));
        strcpy(ignore_list[found_ignores], line);
        ++found_ignores;
    }
    fclose(fp);
    free(line);
}

void build(Command *cmd) {
    char *command_string = NULL;

    for(int i = 0; i < listUsed(cmd->command_list); ++i){
        if (command_string == NULL) {
            command_string = (char *)malloc(strlen(listGet(cmd->command_list, i)) + 1);
        } else {
            command_string = (char *)realloc(command_string, strlen(command_string) + strlen(listGet(cmd->command_list, i)) + 1);
        }

        strcat(command_string, " ");
        strcat(command_string, listGet(cmd->command_list, i));
    }

    printf("%s", command_string);
}

int main(void) {
    Command cmd;
    initList(cmd.command_list);

    add_flag(&cmd, 1, "gcc");
    add_flag(&cmd, 1, "-g");
    add_flag(&cmd, 2, "-o", "test");
    
    add_directory(&cmd, ".");

    build(&cmd);    

    freeList(cmd.command_list);
    
    return 0;
}

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

#include "file.h"

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

index_t filecount = 0, matchcount = 0;

char** extract_ext(char* filename)
{
    index_t len = strlen(filename);
    index_t idx = len - 1;
    index_t ext_len;
    char curr_char;
    static char* file_info[2];

    while (idx >= 0)
    {
        curr_char = filename[idx];
        if (curr_char == ext_limiter)
        {
            // don't include the limiter
            ext_len = len - idx - 1;

            file_info[0] = malloc(len - ext_len);
            file_info[1] = malloc(ext_len);

            strncpy(file_info[0], filename, idx);
            file_info[0][idx] = 0;
            strncpy(file_info[1], filename + idx + 1, ext_len);
            file_info[1][ext_len] = 0;

            //printf("%s %s\n", file_info[0], file_info[1]);
            break;
        }
        --idx;
    }

    return file_info;
}

char* file_name(char* path)
{
    index_t len = strlen(path);
    index_t idx = len - 1;
    char* name = path;

    while (idx >= 0)
    {
        // cross platform :)
        if (path[idx] == win_path || path[idx] == unix_path)
        {
            name = &path[idx + 1];
            break;
        }
        --idx;
    }

    return extract_ext(name)[0];
}

void display_stats()
{
    printf("\nSearched %s%d%s files and found %s%d%s matches\n",
           ANSI_COLOR_GREEN,
           filecount,
           ANSI_COLOR_RESET,
           ANSI_COLOR_GREEN,
           matchcount,
           ANSI_COLOR_RESET);
}

void search_in_file(char* filename, const char* search, short recursive, short verbose)
{
    // open file and search for search_in_file
    FILE* file = fopen(filename, "r");
    int line_num = 1;
    char temp[512];
    if(file == NULL) {
        // is a directory
        if (recursive)
            iterate_files(filename, search, 0, recursive, verbose);
        return;
    }
    ++filecount;
    while(fgets(temp, 512, file) != NULL) {

        char* found = (char *) strstr(temp, search);
        if(found) {
            size_t column = found - temp;
            size_t search_len = strlen(search);
            size_t line_len = strlen(temp);

            char* before_match;
            if (column == 0)
                before_match = "";
            else
            {
                before_match = strncpy(malloc(column), temp, column);
                before_match[column] = 0;
            }

            char* after_match = temp + column + search_len;
            after_match[search_len - line_len] = 0;

            printf("%s> Line %4d, Char %4d: ", filename, line_num, column);
            printf("%s%s%s%s%s", before_match, ANSI_COLOR_GREEN, search, ANSI_COLOR_RESET, after_match);
            ++matchcount;
        }
        line_num++;
    }
}

void iterate_files(const char* cur_dir, const char* search, short outer, short recursive, short verbose) {
    DIR *d;
    struct dirent *dir;
    d = opendir(cur_dir);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(dir->d_name[0] != '.')
            {
                char* path = strcpy(malloc(sizeof(cur_dir) + sizeof(dir->d_name)), cur_dir);
                search_in_file(strcat(strcat(path, "/"), dir->d_name),
                               search, recursive, verbose);
            }
        }
        closedir(d);
    }
    if (outer && verbose)
        display_stats();
}



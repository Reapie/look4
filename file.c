#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

#include "file.h"

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

index_t filecount = 0, matchcount = 0;

char **extract_ext(char *filename)
{
    index_t len = strlen(filename);
    index_t idx = len - 1;
    index_t ext_len;
    char curr_char;
    static char *file_info[2];

    // values if no extension exists
    file_info[0] = filename;
    file_info[1] = "";

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

            break;
        }
        --idx;
    }

    return file_info;
}

char *file_name(char *path)
{
    index_t len = strlen(path);
    index_t idx = len - 1;
    char *name = path;

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
    printf("Searched %s%d%s files and found %s%d%s matches\n",
           ANSI_COLOR_GREEN,
           filecount,
           ANSI_COLOR_RESET,
           ANSI_COLOR_GREEN,
           matchcount,
           ANSI_COLOR_RESET);
}

char *copy_to_lower(char *str, char *new_str)
{
    index_t i = 0;
    while (str[i] != 0)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            new_str[i] = str[i] + 32;
        }
        else
        {
            new_str[i] = str[i];
        }
        ++i;
    }
    new_str[i] = 0;
    return new_str;
}

int find_offset(const char *str, const char *substr)
{
    index_t len = strlen(str);
    index_t sub_len = strlen(substr);
    int idx = 0;

    while (idx < len)
    {
        if (strncmp(str + idx, substr, sub_len) == 0)
            return idx;
        ++idx;
    }

    return -1;
}

void search_in_file(FILE *file, char *filename, const char *search, short ignore_case)
{
    int line_num = 1;
    char original[512];
    char line[512];
    while (fgets(original, 512, file) != NULL)
    {
        // search string already in lowercase
        if (ignore_case)
            copy_to_lower(original, line);
        else
            strncpy(line, original, 512);

        int found = find_offset(line, search);
        if (found != -1)
        {
            size_t search_len = strlen(search);
            size_t line_len = strlen(line);

            char *before_match;
            short freeable = 0;

            // match is at the beginning of the line
            if (!found)
                before_match = "";
            else
            {
                before_match = malloc(found);
                strncpy(before_match, original, found);
                before_match[found] = 0;
                freeable = 1;
            }

            char *match = malloc(search_len);
            match = strncpy(match, original + found, search_len);
            match[search_len] = 0;

            char *after_match = original + found + search_len;
            after_match[search_len - line_len] = 0;

            printf("%s> Line %4d, Char %4d: ", filename, line_num, found);
            printf("%s%s%s%s%s", before_match, ANSI_COLOR_GREEN, match, ANSI_COLOR_RESET, after_match);

            // add a line break if current line doesnt have one
            if (after_match[strlen(after_match) - 1] != 0xa)
                printf("\n");

            // free memory if able to
            if (freeable)
                free(before_match);

            free(match);
            ++matchcount;
        }
        ++line_num;
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void iterate_files(const char *cur_dir, const char *search, short outer, int flags)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(cur_dir);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            // dont include files or directories starting with .
            if (dir->d_name[0] != '.')
            {
                // get the full path
                char *filename = strcpy(malloc(sizeof(cur_dir) + sizeof(dir->d_name)), cur_dir);
                filename = strcat(strcat(filename, "/"), dir->d_name);
                FILE *file = fopen(filename, "r");
                if (file == NULL)
                {
                    // is a directory so recurse if enabled
                    if (flags & flag_recursive)
                        iterate_files(filename, search, 0, flags);
                }
                else
                {
                    search_in_file(file, filename, search, flags & flag_ignore_case);
                    ++filecount;
                    fclose(file);
                    free(filename);
                }
            }
        }
        closedir(d);
    }

    // only show stats if we are at the outermost level and verbose is enabled
    if (flags & flag_verbose && outer)
        display_stats();
}

#pragma clang diagnostic pop



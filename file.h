#ifndef LOOK4_FILE_H
#define LOOK4_FILE_H

typedef unsigned long long index_t;

static const int flag_verbose = 0b001;

static const int flag_recursive = 0b010;

static const int flag_ignore_case = 0b100;

static const int ext_limiter = '.';

static const int win_path = '\\';

static const int unix_path = '/';

// Returns the file name from a path.
char *file_name(char *path);

// Returns an array of file basename and extension.
char **extract_ext(char *filename);

// iterates over all files starting from a directory, can be recursive and call itself on subdirectories
void iterate_files(const char *cur_dir, const char *search, short outer, int flags);

char* copy_to_lower(char *str, char* new_str);

#endif //LOOK4_FILE_H

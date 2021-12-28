#ifndef LOOK4_FILE_H
#define LOOK4_FILE_H

typedef unsigned long long index_t;

static const int ext_limiter = '.';

static const int win_path = '\\';

static const int unix_path = '/';

// Returns the file name from a path.
char *file_name(char *path);

// Returns an array of file basename and extension.
char **extract_ext(char *filename);

void iterate_files(const char *cur_dir, const char *search, short outer, short recursive, short verbose);

#endif //LOOK4_FILE_H

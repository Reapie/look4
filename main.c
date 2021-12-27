#include <stdio.h>
#include <string.h>

#include "file.h"

void print_help(char* filename)
{
    printf("Usage: %s <string to look for>\n", file_name(filename));
    printf("Will search all files in the current directory for the string.\n");
    printf("If the -r flag is given, will search recursively.\n");
    printf("If the -v flag is given, will print a summary of scanned files and matches.\n");
    printf("If the -h flag is given, will print this help message.\n");
}

int main(int argc, char** argv) {
    short recursive = 0;
    short verbose = 0;
    char* search = NULL;
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    } else {
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "-r") == 0)
            {
                recursive = 1;
            }
            else if (strcmp(argv[i], "-v") == 0)
            {
                verbose = 1;
            }
            else if (strcmp(argv[i], "-h") == 0)
            {
                print_help(argv[0]);
                return 0;
            } else
            {
                search = argv[i];
            }
        }
        if (search == NULL)
        {
            print_help(argv[0]);
            return 1;
        }
        iterate_files(".", search, 1, recursive, verbose);
    }
    return 0;
}





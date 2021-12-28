#include <stdio.h>
#include <string.h>

#include "file.h"

void print_help(char *filename)
{
    printf("Usage: %s <string to look for>\n", file_name(filename));
    printf("Will search all files in the current directory for the string.\n");
    printf("If the -r flag is given, will search recursively.\n");
    printf("If the -v flag is given, will print a summary of scanned files and matches.\n");
    printf("If the -h flag is given, will print this help message.\n");
}

int main(int argc, char **argv)
{
    short recursive = 0;
    short verbose = 0;
    char *search = NULL;
    if (argc < 2)
    {
        print_help(argv[0]);
        return 1;
    }
    else
    {
        for (int arg = 1; arg < argc; ++arg)
        {
            if (argv[arg][0] == '-')
            {
                // iterate over argument
                index_t idx = 1;
                while (argv[arg][idx])
                {
                    switch (argv[arg][idx])
                    {
                        case 'r':
                            recursive = 1;
                            break;
                        case 'v':
                            verbose = 1;
                            break;
                        case 'h':
                            print_help(argv[0]);
                            return 0;
                        default:
                            printf("Unknown flag: %c\n", argv[arg][idx]);
                            return 1;
                    }
                    ++idx;
                }
            }
            else
            {
                search = argv[arg];
            }
        }
        if (search == NULL)
        {
            print_help(argv[0]);
            // debugging
            // iterate_files(".", "UwU", 1, 1, 1);
            return 1;
        }
        else
        {
            iterate_files(".", search, 1, recursive, verbose);
        }
    }
    return 0;
}





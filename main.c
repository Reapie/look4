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
    // int for extra size
    int flags = 0;
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
            // is arg a flag (or option)
            if (argv[arg][0] == '-')
            {
                // iterate over chars of flag
                index_t idx = 1;
                while (argv[arg][idx])
                {
                    switch (argv[arg][idx])
                    {
                        case 'r':
                            flags |= flag_recursive;
                            break;
                        case 'v':
                            flags |= flag_verbose;
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
            else // not a flag, must be search string
            {
                search = argv[arg];
            }
        }
        if (search == NULL)
        {
            print_help(argv[0]);
            return 1;
        }
        else
        {
            iterate_files(".", search, 1, flags);
        }
    }
    return 0;
}





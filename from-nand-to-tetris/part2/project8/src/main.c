#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "interpreter.h"
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

bool is_directory(char * filename);

char * generate_output_filename(char * filename);

int main(int argc, char * argv[])
{
    if (2 == argc)
    {
        char * filename = argv[1];
        if (is_directory(filename))
        {
            printf("file %s is a directory.\n", filename);
            DIR * dir = opendir(filename);
            struct dirent* dir_entry;

            while((dir_entry = readdir(dir)))
            {
                if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..") &&
                        (NULL != strstr(dir_entry->d_name, ".vm")) )
                {
                    printf("processing file %s\n", dir_entry->d_name);
//                    PSCANNER scanner = scanner_create(dir_entry->d_name);
//                    scanner_destroy(scanner);
                }
            }
        }
        else
        {
            PSCANNER scanner =  scanner_create(filename);

            char * out_filename = (char*)malloc(sizeof(char) * strlen(filename) + 1);
            strcpy(out_filename, filename);
            char *p = strrchr(out_filename, '.');
            strcpy(p, ".asm");

            if (NULL != scanner)
            {
                PLEXER lexer = lexer_create(scanner);
                if (NULL != lexer)
                {
                    PPARSER parser = parser_create(lexer);
                    if (NULL != parser)
                    {
                        PINTERPRETER interpreter = interpreter_create(parser);
                        if (NULL != interpreter)
                        {
                            interpreter_interpret(interpreter);
                            interpreter_save_to_file(interpreter, out_filename);
                            interpreter_destroy(interpreter);
                        }
                        parser_destroy(parser);
                    }
                }
                scanner_destroy(scanner);
            }
            free(out_filename);
        }
    }
    else
    {
        printf("usage: VMTranslator filename\n");
    }
    return EXIT_SUCCESS;
}

char * generate_output_filename(char * filename)
{
	char * out_filename = (char*)malloc(sizeof(char) * strlen(filename) + 5);
	strcpy(out_filename, filename);
	char *p = strrchr(out_filename, '.');
	if (p)
		strcpy(p, ".asm");
	else
	{
		if (out_filename[strlen(out_filename)-1] == '/')
			strcpy(out_filename + strlen(out_filename) - 1, ".asm");
		else
			strcat(out_filename, ".asm");
	}
	return out_filename;
}

bool is_directory(char * filename)
{
    bool result = false;
    struct stat file_info;

    if (!stat(filename, &file_info))
    {
        result = S_ISDIR(file_info.st_mode);
    }
    return result;
}

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

bool is_directory(char * filename);

char * generate_output_filename(char * filename, bool is_dir);

char * generate_input_filename(char * dir_name, char * file_name);

int main(int argc, char * argv[])
{
    if (2 == argc)
    {
        char * filename = argv[1];
        if (is_directory(filename))
        {
            char * out_filename = generate_output_filename(filename, true);
            DIR * dir = opendir(filename);
            struct dirent* dir_entry;
			//bool do_bootstrap = true;
            while((dir_entry = readdir(dir)))
            {
                if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..") &&
                        (NULL != strstr(dir_entry->d_name, ".jack")) )
                {
					char * input_filename = generate_input_filename(filename, dir_entry->d_name);

                    PSCANNER scanner = scanner_create(input_filename);
                    if (scanner)
                    {
                        PLEXER lexer = lexer_create(scanner);
                        if (lexer)
                        {
						
                            PPARSER parser = parser_create(lexer);
                            if (parser)
                            {
								parser_execute(parser);
								/*
                                PINTERPRETER interpreter = interpreter_create(parser);
                                if (interpreter)
                                {
                                    interpreter_interpret(interpreter, do_bootstrap); do_bootstrap = false;
                                    interpreter_save_to_file(interpreter, out_filename);
                                    interpreter_destroy(interpreter);
                                }
								*/
                                parser_destroy(parser);
                            }
                            lexer_destroy(lexer);
                        }
                        scanner_destroy(scanner);
                    }
                }
            }
			free(out_filename);
        }
        else
        {
            PSCANNER scanner =  scanner_create(filename);

			char * out_filename = generate_output_filename(filename, false);

            if (NULL != scanner)
            {
                PLEXER lexer = lexer_create(scanner);
                if (NULL != lexer)
                {
                    PPARSER parser = parser_create(lexer);
                    if (NULL != parser)
                    {
						parser_execute(parser);
						/*
                        PINTERPRETER interpreter = interpreter_create(parser);
                        if (NULL != interpreter)
                        {
                            interpreter_interpret(interpreter, false);
                            interpreter_save_to_file(interpreter, out_filename);
                            interpreter_destroy(interpreter);
                        }
						*/
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
        printf("usage: JackAnalyzer (directory | filename)\n");
    }
    return EXIT_SUCCESS;
}

char * generate_output_filename(char * filename, bool is_dir)
{
	char * out_filename = NULL;
	if (is_dir)
	{
		char * p = filename + strlen(filename) - 1;
		if (*p == '/')
		{
			*p = '\0';
		}
		p = strrchr(filename, '/');
		if (p)
		{
			out_filename = (char*)malloc(sizeof(char) * (strlen(filename) + strlen(p) + 5));
			strcpy(out_filename, filename);
			strcat(out_filename, "/");
			strcpy(out_filename + strlen(out_filename), p + 1);
			strcat(out_filename, ".xml");
		}
		else
		{
			out_filename = (char*)malloc(sizeof(char) * 2 * strlen(filename) + 6);
			strcpy(out_filename, filename);
			strcat(out_filename, "/");
			strcat(out_filename, filename);
			strcat(out_filename, ".xml");
		}
	}
	else
	{
		out_filename = (char*)malloc(sizeof(char) * strlen(filename) + 1);
		strcpy(out_filename, filename);
		char *p = strrchr(out_filename, '.');
		strcpy(p, ".xml");

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

char * generate_input_filename(char * dir_name, char * file_name)
{
	uint16_t file_size = strlen(dir_name) + strlen(file_name) + (dir_name[strlen(dir_name)-1] != '/' ? 2 : 1);
	
	char * input_filename = (char*)malloc(sizeof(char) * file_size);

	strcpy(input_filename, dir_name);

	if (dir_name[strlen(dir_name)-1] != '/')
		strcat(input_filename, "/");

	strcat(input_filename, file_name);

	return input_filename;
}





#ifndef EXEPTIONS_H
#define EXEPTIONS_H


typedef enum errors{
	CORRECT = 0,
	NAME_ERROR ,  
	DONT_EXIST_FILE_ERROR ,
	SYNTAX_ERROR, 
	TOO_LONG_LINE_COMMAND_ERROR, 
	ALLOCATION_ERROR/*,
	LENGTH_LABEL_ERROR*/ 
}err;

/*prints the error if there is some*/ 
void print_errors(int, int);

/*check the name of the file and if the file is empty.*/
int name_check_file(char* file_name, char *the_name);

/*check the syntax of the line*/
int check_line_syntax(char*);

#endif

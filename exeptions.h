#ifndef EXEPTIONS_H
#define EXEPTIONS_H

/*all the possible errors*/
typedef enum errors{
	GET_NUM_OF_ERRORS = 999,
	EQUATE_TO_ZERO = 998,
	CORRECT = 0,

	/*file*/	
	NAME_ERROR,  
	DONT_EXIST_FILE_ERROR,

	/*line*/
	SYNTAX_ERROR, 
	TOO_LONG_LINE_COMMAND_ERROR, 
	
	/*label*/
	LENGTH_LABEL_ERROR,
	LABEL_DONT_EXIST,
	LABEL_COLON_ERROR,
	LABEL_ALREADY_EXIST_ERROR,
	LABEL_SYSTEM_WORD_ERROR,

	/*operands*/		
	TOO_MANY_OPERANDS,
	REGISTER_DONT_EXIST,
	KEY_WORD_INCORRECT,
	EXTERNAL_ENTRY_TOGETHER_ERROR,
	SIZE_NUMBER_ERROR,

	/*allocation*/
	ALLOCATION_ERROR	
}err;

/*prints the error if there is some*/ 
void print_errors(int error_type, int num_of_line, int *exeptions, char *note);

/*check the name of the file and if the file is empty.*/
int name_check_file(char* file_name, char *the_name);





#endif

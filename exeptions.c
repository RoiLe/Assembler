#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "exeptions.h"



 

void print_errors(int error_type, int num_of_line, int *exeptions, char *note)
{ 
	static int num_of_errors = 0;
  
	switch(error_type)
    	{ 
		case GET_NUM_OF_ERRORS: 
            		*exeptions = num_of_errors;
            		break;
	
		case EQUATE_TO_ZERO: 
            		num_of_errors = 0;
            		break;

		/*file*/
        	case NAME_ERROR: 
            		printf("ERROR: '%s' file name is incorrect.\n", note);
			num_of_errors++;
            		break;
 
 		case DONT_EXIST_FILE_ERROR: 
            		if(num_of_errors == CORRECT){printf("ERROR: '%s' file is not exist.\n", note);}
			num_of_errors++;
            		break;

		/*line*/
		case SYNTAX_ERROR: 
            		printf("ERROR: line %d: syntax error <%s>\n", num_of_line, note);
			num_of_errors++;
            		break;

		case TOO_LONG_LINE_COMMAND_ERROR: 
            		printf("ERROR: line %d: too long line.\n", num_of_line);
			num_of_errors++;
            		break;

		
		/*label*/ 
		case LENGTH_LABEL_ERROR:
			printf("ERROR: line %d: too long label.\n", num_of_line);
			num_of_errors++;
            		break;
      		
		case LABEL_DONT_EXIST: 
            		printf("ERROR: line %d: '%s' label not founded\n", num_of_line, note);
			num_of_errors++;
            		break;
		
		case LABEL_ALREADY_EXIST_ERROR:
			printf("ERROR: line %d: the label '%s' already exist.\n",  num_of_line, note);
			num_of_errors++;
            		break;
		
 		case LABEL_SYSTEM_WORD_ERROR:
			printf("ERROR: line %d: The label '%s' is belongs to the system.\n",  num_of_line, note);
			num_of_errors++;
            		break;
		

		/*operands*/
		case TOO_MANY_OPERANDS:  
            		printf("ERROR: line %d: extraneous operand.\n", num_of_line);
			num_of_errors++;
            		break;

		case REGISTER_DONT_EXIST: 
            		printf("ERROR: line %d: register dont exist.\n", num_of_line);
			num_of_errors++;
            		break;
		
		case KEY_WORD_INCORRECT:
			if(num_of_errors == CORRECT){printf("ERROR:line %d: key word '%s' is incorrect.\n", num_of_line, note);}
			num_of_errors++;
            		break;

		case EXTERNAL_ENTRY_TOGETHER_ERROR: 
            		printf("ERROR: line %d: '%s' already used.\n", num_of_line, note);
			num_of_errors++;
            		break;

		case SIZE_NUMBER_ERROR:
			printf("ERROR: line %d: exceptional numeric value.\n", num_of_line);
			num_of_errors++;
            		break;

		/*allocation*/
		case ALLOCATION_ERROR: 
            		printf("ERROR: there is a problem to allocate memory.\n");
			num_of_errors++;
            		break;	
    	}/*end of switch*/

}/*end printError func*/


 

int name_check_file(char* file_name, char *the_name)
{
	int suffixFlag = off, pointFlag = off, i;
	const char ch[1] = "."; 
	char *checkSuffix; 
	
	for( i= 0; i < strlen(file_name); i++)    /*check if there is dot ['.']*/
	{
		if(file_name[i] == '.')
		{
			pointFlag = on;
			the_name[i] = '\0';
			break;
		}
		the_name[i] = file_name[i];	
	}
	if(pointFlag == off){return NAME_ERROR;}

	checkSuffix = strstr(file_name, ch);	
	suffixFlag = (strcmp(checkSuffix, ".as") == CORRECT)?CORRECT:NAME_ERROR; /*the suffix is incorrect . [not ".as"]*/
	
	return suffixFlag;	

}/*END name_check_file()*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "exeptions.h"





void print_errors(int errorType, int numOfLine)
{
	switch(errorType)
    	{

        case 0: 
            	break;

        case 1: /*name error*/
            	printf("ERROR: the file name is incorrect.\n");
            	break;

 	case 2: /*empty file error*/
            	printf("ERROR: the file is not exist.\n");
            	break;

	case 3: /*sentence syntax error*/
            	printf("ERROR: there is a syntax error in line %d.\n", numOfLine);
            	break;

	case 4: /*sentence syntax error*/
            	printf("ERROR: the line %d is too long.\n", numOfLine);
            	break;

	case 5: /*sentence syntax error*/
            	printf("ERROR: there is a problem to allocate memory.\n");
            	break;
       /*default:*/

    	}

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

}/*END nameCheckFile()*/

/*


*/
int check_line_syntax(char* currLine)
{
	int counter = 0, i;

	for(i = 0; i < strlen(currLine); i++)
	{
		/*add check the label*/
		/*add the guidence syntax check */
		/*add the instruction syntax check */


	}/*end for loop*/

	if((counter == LINE_MAX_LENGTH) && (currLine[counter] != EOF))return SYNTAX_ERROR;
	

	return CORRECT;
}/*END checkLineSyntax()*/

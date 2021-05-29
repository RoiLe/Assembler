

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "exeptions.h"




void printErrors(int errorType)
{
	switch(errorType)
    	{
	case -1: /*the use didnt pu a file*/
		printf("ERROR: there is no file. \n");

        case 0: 
            	break;

        case 1: /*name error*/
            	printf("ERROR: the file name is incorrect.");
            	break;

 	case 2: /*empty file error*/
            	printf("ERROR: the file is not exist.");
            	break;


       /*default:*/

    	}

}/*end printError func*/




/*adds the non "." option!!!!*/
int nameCheckFile(char* fileName)
{
	int pointFlag = CORRECT;
	const char ch[1] = "."; 
	char *checkSuffix; 

	checkSuffix = strstr(fileName, ch);	
	pointFlag = (strcmp(checkSuffix, ".as") == CORRECT)?CORRECT:NAME_ERROR; /*the suffix is incorrect . [not ".as"]*/

	return pointFlag;	

}

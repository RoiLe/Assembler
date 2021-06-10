#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h" 
#include "utils.h"


/*int num_of_lines(FILE*);create!*/


int classification_of_sentence(char* currLine)
{
	int emptyFlag = off, guidanceFlag = off;
	int i = 0;
 	if(currLine[0] == ';'){return NOTE_LINE;}

	while(i < strlen(currLine))
	{
		if(currLine[i] != '\t' || currLine[i] != ' ')
			{emptyFlag = on;}

		if(currLine[i] == '.')
			{guidanceFlag = on;}
		i++;	
	}
	
	if(emptyFlag == off)
		{return EMPTY_LINE;}
	if(guidanceFlag == on)
		{return GUIDANCE_LINE;}
	else
		{return INSTRUCTION_LINE;}
}/*END classificationOfSentence()*/


char *find_the_label(char* currLine)
{
	int  i;
	char *label = (char *)calloc(LABEL_MAX_LENGTH, sizeof(char));
	char tmpString[LABEL_MAX_LENGTH];

	for(i = 0; i < strlen(currLine); i++)
	{
		/*check the label*/
		if(currLine[i] == ':') /*there is a label in the line.*/
		{	

			tmpString[i] = '\0' ;
			strcpy(label, tmpString);
			return label;
		}

		tmpString[i] = currLine[i]; /*if we dont shoure that there is a label.*/
		
	}/*END for loop*/

	return NULL;
}/*END find_the_label() */


char *revese_string(char* str)
{
	char c, *start, *end;

	if(!str || !*str)
		return str;

	start = str;
	end = str+strlen(str)-1;

	while(start < end)
	{
	 c=*start;
	*start=*end;
	*end=c;	
	start++;
	end--;
	}
	return str;
	
}/*END revese_string()*/

/*- כאן נמצא הmain שעובר על הארגומנטים שהוא מקבל. בודק אם אם הארגומנט עבר את התוכנית ללא שגיאות וממשיך הלאה. (ממשיך בין אם עבר ובין אם לא עבר)
- כמו כן פונקציה נוספת אשר מתכללת את התהליך - כלומר, קוראת לפונקציות  first pass ו second pass.*/ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"



int proccessProgram(char*);

int main(int argc, char *argv[]){
	int exeptions;	
	int i;  
	/*if (argc == 1)--> exit from the program*/
	for(i = 1; 0 < argc; i++) 
	{
		exeptions = proccessProgram(argv[i]);    /*the proccess done in succussefuly.*/
		printErrors(exeptions);
		i++;		
	}  
	


return 0;
}


int proccessProgram(char* file)
{
	int ic = IC, dc = DC, c;
	FILE *assemblyCode;
	
	/*checks the file name*/
	int nameFlag = checkNameFile(file);
	if(nameFlag == 1)
	{
		assemblyCode = fopen(file, "r");
	}
	else
	{
		return NAME_ERROR;
	}		
   
	/*firstPass*/
	while(assemblyCode != NULL) 
	{
		firstpass(assemblyCode, symboleTable, dataImage);     		
   
   	}
   	fclose(assemblyCode);
	
	/*secondPass*/
	while(/*the table is didnt done*/)
	{
		secondPass(symboleTable, dataImage);
	}

	outPut(dataImage);
	return 0;
}/*end of proccessProgram()*/










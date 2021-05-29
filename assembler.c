/*- כאן נמצא הmain שעובר על הארגומנטים שהוא מקבל. בודק אם אם הארגומנט עבר את התוכנית ללא שגיאות וממשיך הלאה. (ממשיך בין אם עבר ובין אם לא עבר)
- כמו כן פונקציה נוספת אשר מתכללת את התהליך - כלומר, קוראת לפונקציות  first pass ו second pass.*/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "exeptions.h"
#include "globals.h"
#include "first_pass.h"


int proccessProgram(char*); 

int main(int argc, char *argv[]){
	int exeptions = 0;	
	int i;

	if (argc == 1)
	{
		printErrors(INPUT_ERROR);
		exit(0); 
	}
	for(i = 1; i < argc; ++i) 
	{
		exeptions = proccessProgram(argv[i]);    /*the proccess done in succussefuly.*/
		printErrors(exeptions);
	}  
	


return 0;
}


 int proccessProgram(char* file) 
{
	int ic = IC, dc = DC, c;
	FILE *assemblyCode = NULL;
	int i = 0;
	
	/*checks the file name*/
	int nameFlag = nameCheckFile(file);
	if(nameFlag == CORRECT)
	{
		assemblyCode = fopen(file, "r");
		if(!assemblyCode){return DONT_EXIST_FILE_ERROR;}
	}
	else
	{
		return NAME_ERROR;
	}

	/*firstPass*/
	while(!feof(assemblyCode)) 
	{	
		char *currLine = (char *)calloc(LINE_MAX_LENGTH, sizeof(char));
		fgets(currLine, LINE_MAX_LENGTH, assemblyCode);
		
		fputs(currLine, stdout);/*deletteeee!!!!*/

		/*firstpass(currLine, symTab, dataImg, ic); 
			
		ic += NEXT_ADRESS; the adress is moving to the next place.*/
		free(currLine);    		

   	}
   	fclose(assemblyCode);
	
	/*secondPass
	while(the table is didnt done)
	{
		secondPass(symboleTable, dataImage);
	}*/
	
	/*output*/
	/*outPut(dataImage);*/
	return 0;
}/*end of proccessProgram()*/


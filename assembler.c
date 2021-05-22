/*- כאן נמצא הmain שעובר על הארגומנטים שהוא מקבל. בודק אם אם הארגומנט עבר את התוכנית ללא שגיאות וממשיך הלאה. (ממשיך בין אם עבר ובין אם לא עבר)
- כמו כן פונקציה נוספת אשר מתכללת את התהליך - כלומר, קוראת לפונקציות  first pass ו second pass.*/ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int theProgram(char*);

int main(int argc, char *argv[]){
	int i = 0;
	if(!theProgram(argv[i])) 
	{
	/*continue to the next arg*/
	}  
	else


return 0;
}


int theProgram(char*){





}

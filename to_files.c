#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "utils.h"

/*Create the object file.*/
void object_file(DI_ptr data_image_head, long ic, long dc, char *the_name);

/*Create the external file*/
void external_file(symLine *symbol_table_head,DI_ptr data_image_head, char *the_name);

/*Create the entry file*/
void entry_file(symLine *symbol_table_head, char *the_name);

/*
	Convert every line in data image from binary line to hexadecimal line
	Example: 00000000011001010100100001000000 -> 0100 40 48 65 00 
*/
void create_hexadecimal_line(char *binary_code, char *hexadecimaly_code);

/*
	Turns the line of code to the other side.
	Example: 0100 00 65 48 40 -> 0100 40 48 65 00 
*/
void reverse_hexa_code(char *hexadecimaly_code);




void outputs(symLine *symbol_table_head, DI_ptr data_image_head, long ic, long dc, char *the_name){
	/*instruction lines -> number of bytes */
	ic = ic - IC;	

	/*the object file*/	
	object_file(data_image_head, ic, dc, the_name);

	/*the external file*/
	external_file(symbol_table_head, data_image_head, the_name);

	/*the entry file*/
	entry_file(symbol_table_head, the_name);


}/*END outputs()*/

void object_file(DI_ptr data_image_head, long ic, long dc, char *the_name){
	/*declarations*/
	FILE *obj_file;	
	char curr_object_name[20], hexadecimal_code[15];
	int  temp = NEXT_ADRESS, i = 0; 
	long machine_code_adress = IC;

	/*create the current file name with the correct suffix*/
	strcpy(curr_object_name, the_name);
	strcat(curr_object_name, ".ob");

	/*create the object file*/
	obj_file = fopen (curr_object_name, "w+");

	/*put the counters*/
	fprintf( obj_file, "     %ld %ld" ,ic, dc );

	/*go through the whole data image */
	while(data_image_head != NULL)
	{	
		/*create the line in hexadecimal*/	
		create_hexadecimal_line(data_image_head -> machineCode, hexadecimal_code);

		while(hexadecimal_code[i] != '\0')
		{
			/*put the adress in the file*/
			if(temp == NEXT_ADRESS)
			{
				fprintf(obj_file, "\n%04ld " , machine_code_adress);
				machine_code_adress += NEXT_ADRESS;
				temp = 0;
			}
			/*put in the file the hexadecimal code*/
			fprintf(obj_file, "%c%c ", hexadecimal_code[i], hexadecimal_code[i + 1]);
			i=i+3;
			temp++;
			
		}/*end inner while loop*/
		
		/*reset the counter*/
		i = 0;
		data_image_head = data_image_head -> next;
	}/*end while loop */

	fclose(obj_file);
}/*END object_file()*/


void external_file(symLine *symbol_table_head, DI_ptr data_image_head, char *the_name){
	/*declarations*/
	FILE *ext_file;
	char curr_external_name[20], *ret, *ret2;
	int boolean_external_comparing = 0, boolean_symbol_comparing  = 0;
	DI_ptr data_image_pointer  = data_image_head;

	/*create the current file name with the correct suffix*/
	strcpy(curr_external_name, the_name);
	strcat(curr_external_name, ".ext");
	

	/*create the external file*/
	ext_file = fopen (curr_external_name, "w+");

	/*go through the whole symbol table */
	while(symbol_table_head != NULL)
	{
		ret = strstr(symbol_table_head -> attribute, "external");
		boolean_external_comparing  = compare_strings(ret, "external");

		if(ret != NULL && boolean_external_comparing )
		{	
			while(data_image_pointer != NULL)
			{
				ret2 = strstr(data_image_pointer  -> sourceCode, symbol_table_head -> symbol);
				/*comparing between the attributes*/
				boolean_symbol_comparing = compare_strings(ret2, symbol_table_head -> symbol);
				if(ret2 != NULL && boolean_symbol_comparing )
					/*put the adress and the label in the file */
					{fprintf( ext_file, "%s %04ld\n" ,symbol_table_head -> symbol, data_image_pointer -> adress);}
				data_image_pointer = data_image_pointer -> next;
			}/*end inner while loop*/			
		}
		/*get to the next place*/
		symbol_table_head = symbol_table_head -> next;
		data_image_pointer = data_image_head;
	}/*end while loop*/

	fclose(ext_file);
}/*END external_file()*/


void entry_file(symLine *symbol_table_head, char *the_name){
	/*declarations*/
	FILE *ent_file;
	char curr_entry_name[20], *ret;
	int boolean_comparing = 0;

	/*create the current file name with the correct suffix*/
	strcpy(curr_entry_name, the_name);
	strcat(curr_entry_name, ".ent");	
	
	/*create the entry file*/
	ent_file = fopen (curr_entry_name, "w+");

	/*go through the whole symbol table */
	while(symbol_table_head != NULL)
	{
		/*compare the attributes*/
   		ret = strstr(symbol_table_head -> attribute, "entry");
		boolean_comparing = compare_strings(ret, "entry");
		if(ret != NULL && boolean_comparing)
			/*put the adress and the label in the file */
			{fprintf( ent_file, "%s %04ld\n" ,symbol_table_head -> symbol, symbol_table_head -> value);}
		
		/*get to the next place*/
		symbol_table_head = symbol_table_head -> next;
	}/*end while loop*/
	
	fclose(ent_file);
}/*END entry_file()*/


void create_hexadecimal_line(char *binary_code, char *hexadecimaly_code){
	/*declarations*/
	char temp_code[MACHINE_CODE_LENGTH];
	int i = 0, j =0 , curr_num = 0, position = 8, max_decimal_num = 9, space_flag = off;

	/*create copy of the binary code*/
	strcpy(temp_code, binary_code);

	/*go through the whole binary code*/
	while(temp_code[i] != '\0')
	{
		/*convertion formula*/
		while(position != 0)
		{					
			curr_num += (temp_code[i] - '0') * position;
			position = position / 2;
			i++;
		}
		position = 8;

		/*bigger than 9 or lower*/
		if(curr_num <= max_decimal_num){hexadecimaly_code[j++] = curr_num + '0';}
		else{hexadecimaly_code[j++] = curr_num + '0' + 7;}		

		/*put space between the hexa numbers*/
		if(space_flag == on){hexadecimaly_code[j++] = ' ';}		
		space_flag = (space_flag == off)?on:off;

		curr_num = 0;
	}/*end while loop*/

	hexadecimaly_code[j++] = '\0';
	/*printf("before: %s\n", hexadecimaly_code);*/

	if(strlen(hexadecimaly_code) > 3){reverse_hexa_code(hexadecimaly_code);}	
	/*printf("after: %s\n", hexadecimaly_code);*/
}/*END create_hexadecimal_line()*/


void reverse_hexa_code(char *hexadecimaly_code){
	/*declarations*/
	int left = 0 , right = strlen(hexadecimaly_code) - 2, next_num = 3;
	char temp, temp_plus;

	/*as long as we didn't pass the middle*/
	while(left < right)
	{
		/*store ine temporary variable*/
		temp = hexadecimaly_code[left];
		temp_plus = hexadecimaly_code[left + 1];

		/*change the left position buy the right*/
		hexadecimaly_code[left] = hexadecimaly_code[right - 1];
		hexadecimaly_code[left + 1] = hexadecimaly_code[right];

		/*change the right position by temp*/
		hexadecimaly_code[right - 1] = temp;
		hexadecimaly_code[right] = temp_plus;

		/*to the next step*/
		left += next_num;
		right -= next_num;
	}/*end while loop*/

}/*reverse_hexa_code()*/




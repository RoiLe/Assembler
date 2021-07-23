#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "utils.h"

void object_file(DI_ptr data_image_head, long ic, long dc, char *the_name);

void external_file(symLine *symbol_table_head,DI_ptr data_image_head, char *the_name);

void entry_file(symLine *symbol_table_head, char *the_name);

void create_hexadecimal_line(char *binary_code, char *hexadecimaly_code);


void outputs(symLine *symbol_table_head, DI_ptr data_image_head, long ic, long dc, char *the_name){

	
	ic = ic - IC;/*instruction lines -> number of bytes */	

	/*the object file*/	
	object_file(data_image_head, ic, dc, the_name);

	/*the external file*/
	external_file(symbol_table_head, data_image_head, the_name);

	/*the entry file*/
	entry_file(symbol_table_head, the_name);


}/*END outputs()*/

void object_file(DI_ptr data_image_head, long ic, long dc, char *the_name){
	FILE *obj_file;	
	char curr_object_name[20], hexadecimal_code[15];

	/*create the current file name with the correct suffix*/
	strcpy(curr_object_name, the_name);
	strcat(curr_object_name, ".ob");

	/*create the object file*/
	obj_file = fopen (curr_object_name, "w+");

	/*put the counters*/
	fprintf( obj_file, "\t%ld %ld\n" ,ic, dc );

	while(data_image_head != NULL)
	{
		fprintf( obj_file, "%04ld\n" , data_image_head -> adress);
		create_hexadecimal_line(data_image_head -> machineCode, hexadecimal_code);
		
		/*printf("binary code: %s\t->\thexa: %s\n", data_image_head -> machineCode, hexadecimal_code);*/
		data_image_head = data_image_head -> next;
	}/*end while loop */
	
	fclose(obj_file);
}/*END object_file()*/


void external_file(symLine *symbol_table_head, DI_ptr data_image_head, char *the_name){
	FILE *ext_file;
	char curr_external_name[20], *ret, *ret2;
	int boolean_external_comparing = 0, boolean_symbol_comparing  = 0;
	DI_ptr data_image_pointer  = data_image_head;

	/*create the current file name with the correct suffix*/
	strcpy(curr_external_name, the_name);
	strcat(curr_external_name, ".ext");
	

	/*create the external file*/
	ext_file = fopen (curr_external_name, "w+");

	while(symbol_table_head != NULL)
	{
		ret = strstr(symbol_table_head -> attribute, "external");
		boolean_external_comparing  = compare_strings(ret, "external");
		/*printf("symbol: %s -> ret: %s \n" ,symbol_table_head -> attribute, ret);*/
		if(ret != NULL && boolean_external_comparing )
		{	
			while(data_image_pointer/*  -> next */!= NULL)
			{
				ret2 = strstr(data_image_pointer  -> sourceCode, symbol_table_head -> symbol);
				boolean_symbol_comparing = compare_strings(ret2, symbol_table_head -> symbol);
				if(ret2 != NULL && boolean_symbol_comparing )
					{fprintf( ext_file, "%s %04ld\n" ,symbol_table_head -> symbol, data_image_pointer -> adress);}
				data_image_pointer = data_image_pointer -> next;
			}/*end inner while loop*/			
		}
		symbol_table_head = symbol_table_head -> next;
		data_image_pointer = data_image_head;
	}/*end while loop*/

	fclose(ext_file);
}/*END external_file()*/


void entry_file(symLine *symbol_table_head, char *the_name){
	FILE *ent_file;
	char curr_entry_name[20], *ret;
	int boolean_comparing = 0;

	/*create the current file name with the correct suffix*/
	strcpy(curr_entry_name, the_name);
	strcat(curr_entry_name, ".ent");	
	
	/*create the entry file*/
	ent_file = fopen (curr_entry_name, "w+");

	while(symbol_table_head /*-> next*/ != NULL)
	{
		
   		ret = strstr(symbol_table_head -> attribute, "entry");
		boolean_comparing = compare_strings(ret, "entry");
		if(ret != NULL && boolean_comparing)
			{fprintf( ent_file, "%s %04ld\n" ,symbol_table_head -> symbol, symbol_table_head -> value);}

		symbol_table_head = symbol_table_head -> next;
	}/*end while loop*/
	
	fclose(ent_file);
}/*END entry_file()*/


void create_hexadecimal_line(char *binary_code, char *hexadecimaly_code){
	char temp_code[MACHINE_CODE_LENGTH];
	int i = 0, j =0 , curr_num = 0, position = 8;
	
	/*strcpy(hexadecimaly_code, "on create.. :)");*//*delete after completing*/

	strcpy(temp_code, binary_code);
	while(temp_code[i] != '\0')
	{
		
		while(position != 0)
		{			
			curr_num += (temp_code[i] - '0') * position;
			position = position / 2;
			i++;
		}
		position = 8;

		if(curr_num < 10){hexadecimaly_code[j++] = curr_num + '0';}
		else{hexadecimaly_code[j++] = curr_num + '0' + 7;}

		curr_num = 0;
	}/*end while loop*/
	hexadecimaly_code[j++] = '\0';

}/*END create_hexadecimal_line()*/










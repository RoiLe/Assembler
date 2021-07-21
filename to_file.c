#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "utils.h"

void object_file(DI_ptr data_image_head, long ic, long dc, char *the_name);

void external_file(symLine *symbol_table_head, char *the_name);

void entry_file(symLine *symbol_table_head, char *the_name);

void outputs(symLine *symbol_table_head, DI_ptr data_image_head, long ic, long dc, char *the_name){
	ic = ic + NEXT_ADRESS - IC;	

	/*the object file*/	
	object_file(data_image_head, ic, dc, the_name);

	/*the external file*/
	external_file(symbol_table_head, the_name);

	/*the entry file*/
	entry_file(symbol_table_head, the_name);


}/*END outputs()*/

void object_file(DI_ptr data_image_head, long ic, long dc, char *the_name){
	FILE *obj_file;
	
	strcat(the_name, ".ob");
	
	obj_file = fopen (the_name, "w+");

	/*put the counters*/
	fprintf( obj_file, "\t%ld %ld\n" ,ic, dc );

	while(data_image_head -> next != NULL)
	{
		fprintf( obj_file, "%04ld\n" , data_image_head -> adress);

		data_image_head = data_image_head -> next;
	}
	
	fclose(obj_file);
}/*END object_file()*/


void external_file(symLine *symbol_table_head, char *the_name){
	FILE *ext_file;
	
	strcat(the_name , ".ext");

	ext_file = fopen (the_name, "w+");

	fprintf( ext_file, "will create soon...");

	while(symbol_table_head -> next != NULL)
	{

		symbol_table_head = symbol_table_head -> next;
	}

}/*END external_file()*/


void entry_file(symLine *symbol_table_head, char *the_name){
	FILE *ent_file;

	strcat(the_name, ".ent");	
	
	ent_file = fopen (the_name, "w+");

	fprintf( ent_file, "will create soon...");

	while(symbol_table_head -> next != NULL)
	{

		symbol_table_head = symbol_table_head -> next;
	}

}/*END entry_file()*/


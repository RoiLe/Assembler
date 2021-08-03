#ifndef TOFILES_H
#define TOFILES_H


/*
	Main program that called the other function to create the files.
	[.ob | .ent | .ext]

	object file -> .ob -> store the machine code.
	enrty file -> .ent -> store all the symbols that difined as entry.
	external file -> .ext -> store all the symbols that difined as external.  
*/
void outputs(symLine *symbol_table_head, DI_ptr data_image_head, long ic, long dc, char *the_name);







#endif

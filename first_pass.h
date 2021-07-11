/*- קובץ Header המחזיק את כל ההגדרות הרלוונטיות לfirstpass.c*/
#ifndef FIRSTPASS_H
#define FIRSTPASS_H



/*
	Creates the data image table. 
	[adress ; source code ; machine code]
	 
	The first part is the instruction lines.
	The second part is the guidances lines.

	**variables**
	curr -> the current line that we add. 
	temp -> pointer that help to store the next line.

	operand_list -> current openrands of guidances line.
	source_code_flag -> help to add the source code just to correct line in the table. 
	lines_counter -> counter of the guidances line (will help forward to put the correct adress).
	curr_adress -> current adress.
	num_of_operands -> number of operands.

*/
int data_image_line_create(DI_ptr *curr_line_ptr, DI_ptr *tail_ptr, char *currLine, long adress, int typeOfSentence, char *key_word, char* operands);

/*
	Creates the symbol table.
	Every label, external, entry values get inside.
	[symbol ; value(adress) ; arrtibute]

	This table will update in the second pass.

	**variables**
	curr_symbol_line -> the current line that we creating.
*/
symLine *add_to_symbol_table(char *label, long adress, int type_of_sentence);

/*
	Connect the adresses in the data image, 
	between instruction line and guidance lines.

	**variables**
	curr_adress -> the current adress. 
*/
void connect_adresses(dataImg *guidance_table, long last_ic);

/*
	Free the symbol table,
	every line. 
	
*/
void free_symbol_table_nodes(symLine *head_symbol_line);

/*
	Free the data image table, 
	every line. 
*/
void free_data_image_nodes(dataImg* head_data_image);





/*vvvvvvv---delete before the submission--vvvvvvvvvvvvv */
void data_image_print(DI_ptr);
void symbol_table_print(symLine*);

#endif

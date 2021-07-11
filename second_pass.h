#ifndef SECONDPASS_H
#define SECONDPASS_H


/*


*/
void second_pass(DI_ptr instruction_head, symLine *symbol_table_head, char *curr_line);


/*
	adds the entry attrubutes to the symbol table, and check if there is a problem with this.

*/
int add_entry_to_symbol_table(symLine *symbol_table_head, char *operand);

/*


*/
void add_missed_values_to_data_image(DI_ptr instruction_head,symLine *symbol_table_head,char *key_word, char *operands, char *curr_line);

/*


*/
int check_reg_flag(DI_ptr head_data_image, char *curr_line);

#endif

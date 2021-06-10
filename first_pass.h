/*- קובץ Header המחזיק את כל ההגדרות הרלוונטיות לfirstpass.c*/
#ifndef FIRSTPASS_H
#define FIRSTPASS_H



/*the proccess of the first passing assembler*/

/*take only the instructions line and organize them in table by the required order*/
void data_image_line_create(DI_ptr *, char*, long);

symLine *add_to_symbol_table( char *, long);

void free_symbol_table_nodes(symLine *);

void free_data_image_nodes(dataImg*);

/*vvvvvvv---delete before the submission--vvvvvvvvvvvvv */
void data_image_print(DI_ptr);
void symbol_table_print(symLine*);/*delete before the submission. */

#endif

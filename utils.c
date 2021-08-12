#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "globals.h" 
#include "utils.h"
#include "exeptions.h"


int classification_of_sentence(char* curr_line)
{
	/*declarations*/
	int label_flag = off;
	int i = 0;
	
	if(curr_line[i] == '\0'){return EMPTY_LINE;}
	
	/*check if there is a label*/
	while(curr_line[i] != '\0')
	{
		if(curr_line[i] == ':'){label_flag = on;}
		i++;
	}/*end while loop */

	i = 0;
	
	if(label_flag == on){while(curr_line[i] != ':'){i++;}}	
	if(curr_line[i] == ':'){i++;}

	while(curr_line[i] == ' ' || curr_line[i] == '\t'){i++;}
	
	if(curr_line[i] == '.'){return GUIDANCE_LINE;}
	if(curr_line[i] == '\n'){return EMPTY_LINE;}
	if(curr_line[i] == ';'){return NOTE_LINE;}

	return INSTRUCTION_LINE;
		
}/*END classificationOfSentence()*/


char *revese_string(char* str)
{
	/*declarations*/
	char c, *start, *end;

	/*it's an empty strign*/
	if(!str || !*str)
		return str;

	start = str;/*first*/
	end = str+strlen(str)-1; /*last*/
	
	/*change every characters in the oposite side*/
	while(start < end)
	{
		c=*start;
		*start=*end;
		*end=c;	
		start++;
		end--;
	}/*end while loop*/

	return str;
	
}/*END revese_string()*/


void get_commands(char *curr_line, char* label, char* key_word, char* operands, int number_line)
{
	/*declarations*/
	enum places {first = 0, second, third};
	int i = 0, label_flag = off, temp_string_length = 0, none = 0;
	char temp_curr_line[LINE_MAX_LENGTH], temp_string[LINE_MAX_LENGTH];
    	
	strcpy(temp_curr_line, curr_line);
	strcat(temp_curr_line, "\0");

		/*--get the first field--*/
		CURR_LINE_WHITE_SPACE_SKIP;
		while(temp_curr_line[i] != ' ' && temp_curr_line[i] != '\n' && temp_curr_line[i] != '\t' && temp_curr_line[i] != ':')
		{
			temp_string[temp_string_length] = temp_curr_line[i];
			temp_string_length++;
			i++;		
		}	
		temp_string[temp_string_length] = '\0';		
	
			/*label scenario*/
		if(temp_curr_line[i] == ':')
		{
			label_flag = on;
			strcpy(label, temp_string);
			temp_string[0] = '\0';
			if(temp_string_length > LABEL_MAX_LENGTH){print_errors(LENGTH_LABEL_ERROR, number_line, &none, NULL);}
			temp_string_length = 0;
			i++;
		
		}/*end label scenario*/

			/*key word scenario*/
		if(temp_curr_line[i] == ' ' || temp_curr_line[i] == '\n' || temp_curr_line[i] == '\t' )
		{
			strcpy(key_word, temp_string);
			temp_string[0] = '\0';
			temp_string_length = 0;
		}/*end key word scenario*/
		
		/*--get the second field--*/
		CURR_LINE_WHITE_SPACE_SKIP;

		if(temp_curr_line[i] == ':')
		{print_errors(SYNTAX_ERROR, number_line, &none, curr_line) ;}
	
			/*key word scenario*/	
		if (label_flag == on)
		{			
			while(temp_curr_line[i] != ' ' && temp_curr_line[i] != '\n' && temp_curr_line[i] != '\t' )
			{
				temp_string[temp_string_length] = temp_curr_line[i];
				temp_string_length++;
				i++;		
			}
			temp_string[temp_string_length] = '\0';
			strcpy(key_word, temp_string);

			temp_string[0] = '\0';
			temp_string_length = 0;
			
			/*operands scenario*/
		}else{
			while(temp_curr_line[i] != '\0')
			{						
				temp_string[temp_string_length] = temp_curr_line[i];
				temp_string_length++;
				i++;
				CURR_LINE_WHITE_SPACE_SKIP;				
			}
			temp_string[temp_string_length] = '\0';
			
			strcpy(operands, temp_string);		

			temp_string[0] = '\0';
			temp_string_length = 0;
		}

		
		/*--(if there is) get the third field--*/
		
			/*operands scenario*/
		if (label_flag == on && strcmp("stop", key_word))
		{
			CURR_LINE_WHITE_SPACE_SKIP;
			
			while(temp_curr_line[i] != '\0')
			{
				temp_string[temp_string_length] = temp_curr_line[i];
				temp_string_length++;
				i++;
				CURR_LINE_WHITE_SPACE_SKIP;		
			}
			
			temp_string[temp_string_length] = '\0';	
			strcpy(operands, temp_string);
			temp_string[0] = '\0';		
		}

}/*END get_commands();*/



int get_instruction_type(char *key_word, int *curr_opcode, int *curr_funct)
{
	/*declarations*/
	/*sizes*/
	int i, num_of_R_logic = 5, num_of_R_copy = 3, num_of_I_logic = 5, num_of_I_branch = 4,
	num_of_I_store_load = 6, num_of_J_special = 4;
		
	
	/*key word arrays*/
	const char *(R_logic_key_word[]) = {"add", "sub", "and", "or", "nor"};	
	const char *(R_copy_key_word[]) = {"move", "mvhi", "mvlo"};
	const char *(I_logic_key_words[]) = {"addi", "subi", "andi", "ori", "nori"};
	const char *(I_branch_key_word[]) = {"bne", "beq", "blt", "bgt"};
	const char *(I_store_load_key_word[]) = {"lb", "sb", "lw","sw", "lh", "sh"};
	const char *(J_special_key_word[]) = {"jmp", "la", "call" , "stop"};
	
	/*funct arrays*/
	const int R_funct_array[] = {FU_ADD, FU_SUB, FU_AND, FU_OR, FU_NOR, FU_MOVE, FU_MVHI, FU_MVLO};
	
	/*opcode arrays*/
 	const int R_opcode_array[] = {OP_ADD, OP_SUB, OP_AND, OP_OR, OP_NOR, OP_MOVE, OP_MVHI, OP_MVLO};
	const int I_opcode_array[] = {OP_ADDI, OP_SUBI, OP_ANDI, OP_ORI, OP_NORI, OP_BNE, OP_BEQ, OP_BLT, OP_BGT, OP_LB, OP_SB, OP_LW, OP_SW, OP_LH, OP_SH};
	const int J_opcode_array[] = {OP_JMP,OP_LA,OP_CALL ,OP_STOP};


	/*All possible scenarios*/
	for(i = 0; i < num_of_R_logic; i++)
	{
		if(!strcmp(key_word, R_logic_key_word[i]))
		{
			*curr_opcode = R_opcode_array[i];
			*curr_funct = R_funct_array[i];
			return R_LINE_LOGIC;
		}
	}

	for(i = 0; i < num_of_R_copy; i++)
	{
		if(!strcmp(key_word, R_copy_key_word[i]))
		{
			*curr_opcode = R_opcode_array[i + num_of_R_logic];
			*curr_funct = R_funct_array[i + num_of_R_logic];
			return R_LINE_COPY;
		}
	}
	
	for(i = 0; i < num_of_I_logic; i++)
	{
		if(!strcmp(key_word, I_logic_key_words[i]))
		{
			*curr_opcode = I_opcode_array[i];
			return I_LINE_LOGIC;
		}
	}
	
	for(i = 0; i < num_of_I_branch; i++)
	{
		if(!strcmp(key_word,I_branch_key_word[i]))
		{
			*curr_opcode = I_opcode_array[i + num_of_I_logic];
			return I_LINE_BRANCH;
		}
	}

	for(i = 0; i < num_of_I_store_load; i++)
	{
		if(!strcmp(key_word,I_store_load_key_word[i]))
		{
			*curr_opcode = I_opcode_array[i + num_of_I_logic + num_of_I_branch];
			return I_LINE_SORE_LOAD;
		}
	}

	for(i = 0; i < num_of_J_special; i++)
	{
		if(!strcmp(key_word,J_special_key_word[i]))
		{	
			*curr_opcode = J_opcode_array[i];
			return J_LINE_SPECIAL;
		}
	}
	
	/*the key word didn't found*/	
	return FALSE;
}/*END  get_key_word_type();*/


int get_guidance_type(char *key_word)
{
	/*declarations*/
	int i, num_of_types = 6;
	const char *(guidance_type[]) = {".db",".dh",".dw",".asciz", ".entry", ".extern"};
	
	/*go through the whole possible options*/
	for(i = 0; i < num_of_types; i++)
	{
		if(!strcmp(key_word,guidance_type[i]))return i + 1; 

	}
	
	return FALSE;
}/*END get_guidance_type()*/


int to_ascii_list_operands(char* operands, int *the_list, int number_line)
{
	/*declarations*/
	int i, quote_flag = off, operands_counter = 0, comma_flag = off,minus_flag = off, temp_num = 0, none; 

	/*check the quotes*/
	for (i = 0; i < strlen(operands); i++)	
	{
		OPERANDS_WHITE_SPACE_SKIP;
		if(operands[i] == '"')
		{
			quote_flag = on;
			i++;
		}			
		break;
	}/*end for loop*/

	/*the .asciz scenario*/
	if(quote_flag == on)
	{	
		/*the first operand*/	
		for (; i < strlen(operands); i++)
		{

			if(operands[i] == '"')
			{
				quote_flag = off;
				break;			
			}
			the_list[operands_counter++] = operands[i];
			OPERANDS_WHITE_SPACE_SKIP;
		}
		the_list[operands_counter++] = 0;/*adds zero line.*/
		if(quote_flag == on){print_errors(SYNTAX_ERROR, number_line, &none, operands);}
	/*the .db .dh.dw scenario.*/
	}else if((operands[i] >= '0' && operands[i] <= '9')){		
		for (; i < strlen(operands);)/*continue from the place that we done in the last loop*/
		{
			OPERANDS_WHITE_SPACE_SKIP;	
			if(operands[i] == ',')
			{	
				if(comma_flag == off){print_errors(SYNTAX_ERROR, number_line, &none, operands);}
				comma_flag = off;
				i++;
			}
			else if(comma_flag == off)
			{			
				do{	/*it's minus or it's two digits number.*/
					if(operands[i] == '-'){minus_flag = on;}
					else{temp_num = operands[i] - 48 + (temp_num * 10);}
					i++;
				}while(operands[i] == '-' ||(operands[i] >= '0' && operands[i] <= '9'));				
				
				comma_flag = on;

				if(minus_flag == on){temp_num = (-1) * temp_num ;}
				the_list[operands_counter] = temp_num;/*update the number to the list*/
				operands_counter++;
				temp_num = 0;	
				OPERANDS_WHITE_SPACE_SKIP;	
			}					

		}/*end of for loop*/	

	}else{/*the .extern .entern scenario*/
		the_list[operands_counter] = operands[i];
	}
	return operands_counter; 
}/*END to_ascii_list_operands()*/


void get_last_field(char *operands, char* label)
{
	/*declarations*/
	int i = 0, end_array = 0;
	char *p = strtok (operands, " ,\t\n");
   	char *array[3];

	/*the deviosion*/
   	while (p != NULL)
    	{
        	array[i++] = p;
       		p = strtok (NULL, " ,\t\n");
		end_array++;
   	}/*end while loop*/	

	strcpy(label, array[end_array - 1]);
	label[strlen(label)] = '\0';

}/*END get_last_field()*/


int compare_strings(char *str1, char *str2){
	/*declaration*/
	int i = 0;

	/*if one of them is NULL*/
	if(str1 == NULL || str2 == NULL){return FALSE;}

	/*as long as both are not over yet*/
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if(str1[i] !=  str2[i]){return FALSE;}
		i++;
	}/*end while loop*/

	/*equals*/
	return TRUE;
}/*END compare_strings() */


long check_the_label_exist(symLine *symbol_table_head, char *label){
	/*go through the whole symbol table */
	while(symbol_table_head != NULL)
	{
		/*if the label exist*/
		if(!strcmp(symbol_table_head -> symbol, label)){return symbol_table_head -> value;}
		symbol_table_head = symbol_table_head -> next; 
	}/*end while loop*/

	return FALSE;
}/*END check_the_label_exist()*/




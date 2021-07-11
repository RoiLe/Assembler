#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h" 
#include "utils.h"


int classification_of_sentence(char* curr_line)
{
	/*declarations*/
	int empty_flag = off, guidance_flag = off;
	int i = 0;
 	if(curr_line[0] == ';'){return NOTE_LINE;}

	/*go through the whole line*/
	while(i < strlen(curr_line))
	{
		if(curr_line[i] != '\t' || curr_line[i] != ' ')
			{empty_flag = on;}

		if(curr_line[i] == '.')
			{guidance_flag = on;}
		i++;	
	}
	
	if(empty_flag == off)
		{return EMPTY_LINE;}
	if(guidance_flag == on)
		{return GUIDANCE_LINE;}
	else
		{return INSTRUCTION_LINE;}
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


void get_commands(char *curr_line, char* label, char* key_word, char* operands)
{
	/*declarations*/
	enum places {first = 0, second, third};
	int i = 0;
    	char *p = strtok (curr_line, " \t\n");
   	char *array[3];

	/*the deviosion*/
   	while (p != NULL)
    	{
        	array[i++] = p;
       		p = strtok (NULL, " ");/*fix to the correct devision' and fix all the sheets that caouse from is.*/
   	}
 	
	i = first;

	/*if its label.*/  
	if((array[i])[strlen(array[first])-1] == ':' && i == first) 
	{
		strcpy(label, array[i++]);
		label[strlen(label)-1] = '\0';
	}else{label[0] = '\0';}/*there is no label*/

	/*next place in the array (key word)*/
	strcpy(key_word, array[i++]);
	key_word[strlen(key_word)] = '\0';

	/*if there is operands*/
	if(array[i])
	{
		strcpy(operands, array[i]);
		operands[strlen(operands) - 1] = '\0';
	}

	/*if there is a white space before ':' in label case.*/	
	if(operands[first] == ':')/*fix : if there is a whitespace before colon.*/
	{
		printf("exeption white space before colon.\n");/*change to real exeption. */
		/*delete all the values until now*/
		key_word[0] = '\0';
		operands[0] = '\0';
		label[0] = '\0';		
	}
	 
}/*END get_key_word();*/



int get_instruction_type(char *key_word, int *curr_opcode, int *curr_funct)
{
	/*declarations*/
	/*sizes*/
	int i, num_of_R_logic = 5, num_of_R_copy = 3, num_of_I_logic = 5, num_of_I_branch = 4, num_of_I_store_load = 6, num_of_J_special = 4;

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
		if(!strcmp(key_word,guidance_type[i]))return i+1; 

	}
	return FALSE;
}/*END get_guidance_type()*/


int to_ascii_list_operands(char* operands, int *the_list)
{
	/*declarations*/
	int i, quote_flag = off, operands_counter = 0, comma_flag = off,minus_flag = off, temp_num = 0; 

	/*check the quotes*/
	for (i = 0; i < strlen(operands); i++)	
	{
		WHITE_SPACE_SKIP;
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
			the_list[operands_counter] = operands[i];
			operands_counter++;
			WHITE_SPACE_SKIP;
		}

	/*the .db .dh.dw scenario.*/
	}else if((operands[i] >= '0' && operands[i] <= '9')){		
		for (; i < strlen(operands);)/*continue from the place that we done in the last loop*/
		{
			WHITE_SPACE_SKIP;	
			if(operands[i] == ',')
			{	
				if(comma_flag == off){printf("ERROR: there is no comma in the right place");}
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
				WHITE_SPACE_SKIP;	
			}					

		}/*end of for loop*/	

	}else{/*the .extern .entern scenario*/
		the_list[operands_counter] = operands[i];
	}
	return operands_counter; 
}/*END to_ascii_list_operands()*/


void get_last_field(char *operands, char* label)
{
	int i = 0, end_array = 0;
	char *p = strtok (operands, ", \t\n");
   	char *array[3];

	/*the deviosion*/
   	while (p != NULL)
    	{
        	array[i++] = p;
       		p = strtok (NULL, ", \t\n");
		end_array++;
   	}/*end while loop*/	

	strcpy(label, array[end_array - 1]);
	label[strlen(label)] = '\0';

}/*END get_last_field()*/










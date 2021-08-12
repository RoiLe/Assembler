#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "utils.h"
#include "exeptions.h"
#include "binary.h"


/*
	Creates the structure of every line in integer
	return the current R line in R_struct
*/
R create_R_instruction(char *key_word, char *operands, int curr_opcode, int curr_funct,  int line_number, int key_word_type);


void instruction_binary_line(char *machineCode, char *key_word , char *operands,  int line_number)
{
	R curr_R_line;
	I curr_I_line;
	J curr_J_line;
	int curr_opcode = 0, curr_funct = 0;
	  
	int type_of_key_word = get_instruction_type(key_word, &curr_opcode, &curr_funct);
	
	if(!type_of_key_word){print_errors(KEY_WORD_INCORRECT, line_number, off, key_word);}
	
	switch(type_of_key_word)/*call to the correct func depened the type*/
	{
		case R_LINE_LOGIC: case R_LINE_COPY: 

			/*create here the struct of R instruction.. */
			curr_R_line = create_R_instruction(key_word, operands, curr_opcode, curr_funct, line_number, type_of_key_word);

			/*lets make the structure binary.. */
			int_to_binary(machineCode, curr_R_line.size,WORD);
			break;

		case I_LINE_LOGIC: case I_LINE_BRANCH: case I_LINE_SORE_LOAD:

			/*create_I_instruction(key_word, operands, type_of_key_word);*/
			curr_I_line = create_I_instruction(key_word, operands, type_of_key_word, curr_opcode, off, line_number);

			/*lets make the structure binary.. */
			int_to_binary(machineCode, curr_I_line.size,WORD);
			break;
		
		case J_LINE_SPECIAL:
			
			/*create_J_instruction(key_word, operands, type_of_key_word);*/
			curr_J_line = create_J_instruction(key_word, operands, curr_opcode, off, line_number);

			/*lets make the structure binary.. */
			int_to_binary(machineCode, curr_J_line.size,WORD);
			break;
	}/*end switch(type_of_key_word)*/
}/*END createInstruction()*/


void guidance_binary_lines(char *machine_code,char *key_word, long data, char *currLine, int line_number, int type_of_guidance)
{
	/*declarations*/
	DB_CODE curr_db_asciz;
	DH_CODE	curr_dh;
	DW_CODE curr_dw;

	int /*type_of_guidance = get_guidance_type(key_word), */none;	

	/*printf("current line: %s\n", currLine);
	printf("type: %d\n", type_of_guidance);*/
	/*if(type_of_guidance == FALSE){print_errors(KEY_WORD_INCORRECT, line_number, off, key_word);}*/
	switch(type_of_guidance)
	{ 
		case DB:
		case ASCIZ:		
			/*check the size of the number*/
			if(data > MAX_SIGNED_BYTE || data < MIN_SIGNED_BYTE)
			{
				print_errors(SIZE_NUMBER_ERROR, line_number, &none, NULL);				
				break;
			}
			/*Adds the current data */		
			curr_db_asciz.guidance.byte = data;
			/*lets make the structure binary.. */
			int_to_binary(machine_code, curr_db_asciz.size,BYTE);
			break;
		case DH:
			/*check the size of the number*/
			if(data > MAX_SIGNED_HALF_WORD || data <  MIN_SIGNED_HALF_WORD)
			{
				print_errors(SIZE_NUMBER_ERROR, line_number, &none, NULL);				
				break;
			}
			/*Adds the current data */
			curr_dh.guidance.half_word = data;
			/*lets make the structure binary.. */
			int_to_binary(machine_code, curr_dh.size,HALF_WORD);
			break;
		case DW:
			/*Adds the current data */
			curr_dw.guidance.word = data;
			/*lets make the structure binary.. */
			int_to_binary(machine_code, curr_dw.size, WORD);
			break;
		/*default:
			print_errors(KEY_WORD_INCORRECT, line_number, off, key_word);*/
	}/*end of switch*/

	/*return type_of_guidance;*/
}/*END create_guidance_code()*/


void int_to_binary(char *machine_code, int curr, int num_of_byte)
{
	/*declaration*/
	unsigned mask = 1;

	/*go through the mask*/
	for(;mask;mask <<= 1)
	{	
		if(num_of_byte != 0)
		{	
			char bit;
			bit = (mask & curr)? '1':'0'; 	
			strncat(machine_code, &bit, 1);
			num_of_byte--;
		}
	}/*end for loop*/

	/*adds the EOF to the end of the code*/
	strcat(machine_code, "\0");
	
	/*change the side*/
	revese_string(machine_code);
}/*END int_to_binary()*/


R create_R_instruction(char *key_word, char *operands, int curr_opcode, int curr_funct,  int line_number, int key_word_type)
{	
	/*declarations*/
	int i = 0 , comma_flag = off, none = 0, logic_aritmetic_operands = 3, copy_operands = 2, 
	operands_counter = 0, temp_rd = 0, temp_rt = 0, temp_rs = 0,
	num_of_reg = -1, diff_to_int = 48;	
	R curr;


	/*add the correct funct and opcode. */
	curr.instruct.funct = curr_funct;
	curr.instruct.opcode = curr_opcode;

	
	/*add the registers. */
	for(i = 0; i < strlen(operands);)
	{		
		OPERANDS_WHITE_SPACE_SKIP;
		
		if(operands[i] == '$' && comma_flag == off)
		{
			i++;
			comma_flag = on;
			operands_counter++;

			/*operands between 0 to 9.*/
			if(operands[i] >= ZERO && operands[i] <= NINE)
			{
				/*check if its two digit number.*/
				 if(operands[i + 1] >= ZERO && operands[i + 1] <= NINE)
				{
					num_of_reg = ((operands[i] - diff_to_int) * 10) + (operands[i + 1] - diff_to_int);
					i = i + 2;
				}else{
					num_of_reg = operands[i] - diff_to_int;
					i++;
				}
			}else{print_errors(SYNTAX_ERROR, line_number, &none, operands);}

			if(num_of_reg >= MIN_REGISTER && num_of_reg <= MAX_REGISTER)
			{
				switch(operands_counter)/*depends how many operands there is.*/
				{
					case 1:
						temp_rs = num_of_reg;
						break;
					case 2:
						if(curr.instruct.opcode == OP_MOVE){temp_rd = num_of_reg;}						
						else{temp_rt = num_of_reg;}
						break;
					case 3:
						if(curr.instruct.opcode == OP_MOVE)
						{
							print_errors(SYNTAX_ERROR, line_number, &none, operands);
							return curr;
						}
						else{temp_rd = num_of_reg;}	
						break;
					default:
						print_errors(TOO_MANY_OPERANDS, line_number, &none, NULL);
						return curr;

				}/*end switch*/
				num_of_reg = -1;
			}else{print_errors(REGISTER_DONT_EXIST, line_number, &none, NULL);}
		}else{
			if(operands[i++] == ','){comma_flag = off;}
			else{
				print_errors(SYNTAX_ERROR, line_number, &none, operands);
				return curr;
			}
		}	
		OPERANDS_WHITE_SPACE_SKIP;
	 	
	
	}/*end for loop*/		
	
	/*few operands than what exepted*/
	if((key_word_type == R_LINE_LOGIC && operands_counter < logic_aritmetic_operands) ||
	(key_word_type == R_LINE_COPY && operands_counter < copy_operands))
	{print_errors(FEWER_OPERANDS, line_number, &none, NULL);}
	
	/*there is a comma after the line.*/	
	if(comma_flag == off){print_errors(SYNTAX_ERROR, line_number, &none, operands);}

	/*adds the final values.*/
	curr.instruct.rd = temp_rd;
	curr.instruct.rt = temp_rt;
	curr.instruct.rs = temp_rs;

	/*always zero in this scenario. */
	curr.instruct.NONE = 0;	

	return curr;
}/*END create_R_instruction()*/
 

I create_I_instruction(char *key_word, char *operands, int type_of_I, int curr_opcode, long curr_immed, int line_number )
{
	/*declarations*/
	I curr;
	int i, temp_rs = 0, temp_rt = 0, comma_flag = off, minus_flag = off, num_of_operands =2,
	num_of_reg = 0, operands_counter = 0,  diff_to_int = 48, none = 0, temp_immed = 0;

	
	/*add the correct opcode. */
	curr.instruct.opcode = curr_opcode;

	/*add the operands. */
	for(i = 0; i < strlen(operands);)
	{
		OPERANDS_WHITE_SPACE_SKIP;
		
		if(operands[i] == '$' && comma_flag == off)
		{
			i++;
			comma_flag = on;
			operands_counter++;
			
			if(operands[i] >= ZERO && operands[i] <= NINE)
			{
				/*check if its two digit number.*/
				 if(operands[i + 1] >= ZERO && operands[i + 1] <= NINE)
				{
					num_of_reg = ((operands[i] - diff_to_int) * 10) + (operands[i + 1] - diff_to_int);
					i = i+2;
				}else{
					num_of_reg = operands[i] - diff_to_int;
					i++;
				}
			}else{
				print_errors(SYNTAX_ERROR, line_number, &none, operands);
				return curr;
			}

			/*the register exist. */
			if(num_of_reg >= MIN_REGISTER && num_of_reg <= MAX_REGISTER)
			{
				switch(operands_counter)/*depends how many operands there is*/
				{
					case 1:
						temp_rs = num_of_reg;
						break;
					case 2:
						temp_rt = num_of_reg;
						break;
					default:
						print_errors(TOO_MANY_OPERANDS, line_number, &none, NULL);
						return curr;			

				}/*end switch*/
				num_of_reg = -1;
			}else{print_errors(REGISTER_DONT_EXIST, line_number, &none, NULL);}

		}else{ 
			if(operands[i] == ',')
			{
				comma_flag = off;
				i++;
			}
			/*create the immed number value*/
			else if(comma_flag == off)
			{
				while(operands[i] == '-' ||(operands[i] >= ZERO && operands[i] <= NINE))
				{
					if(operands[i] == '-'){minus_flag = on;}
					else{temp_immed = operands[i] -	ZERO + (temp_immed * 10);}
					i++;
				}
				comma_flag = on;
				if(minus_flag == on){temp_immed = (-1) * temp_immed ;}	
				
				/*check if the number is in the range*/
				if(temp_immed > MAX_SIGNED_HALF_WORD && temp_immed < MIN_SIGNED_HALF_WORD)
				{
					print_errors(SIZE_NUMBER_ERROR, line_number, &none, NULL);
					return curr;			
				}		
			}		
			/*its a label*/
			else if((operands[i] >= 'A' && operands[i] <= 'Z') || (operands[i] >= 'a' && operands[i] <= 'z'))
			{
				temp_immed = curr_immed;
				i++;
			}else{
				print_errors(SYNTAX_ERROR, line_number, &none, operands);
				i++;
				return curr;					
			}			
		}
		OPERANDS_WHITE_SPACE_SKIP;

	}/*end for loop*/

	/*few operands than what exepted*/
	if(operands_counter < num_of_operands)
	{print_errors(FEWER_OPERANDS, line_number, &none, NULL);}

	/*check the size of immed value*/
	if(temp_immed > MAX_SIGNED_HALF_WORD|| temp_immed < MIN_SIGNED_HALF_WORD)
	{print_errors(SIZE_NUMBER_ERROR, line_number, &none, NULL);}

	/*there is a comma after the line.*/	
	if(comma_flag == off){print_errors(SYNTAX_ERROR, line_number, &none, operands);}

	/*adds the final values.*/
	curr.instruct.immed = temp_immed;	
	curr.instruct.rs = temp_rs;
	curr.instruct.rt = temp_rt;
	return curr;

}/*END create_I_instruction()*/

J create_J_instruction(char *key_word, char *operands, int curr_opcode, long adress, int line_number)
{
	/*declarations*/
	J curr;		
	int i, temp_reg = off, temp_address = 0, num_of_reg = 0, none;

	/*add the correct opcode. */
	curr.instruct.opcode = curr_opcode;

	/*add the operands. */
	for(i = 0; i < strlen(operands);)
	{
		OPERANDS_WHITE_SPACE_SKIP;
		/*there is a regsiger.*/
		if(operands[i] == '$'){temp_reg = on;}
		i++;

		if(operands[i + 1] >= ZERO && operands[i + 1] <= NINE)/*two digits number*/
		{
			num_of_reg = ((operands[i] - ZERO) * 10) + (operands[i + 1] - ZERO);
			i = i + 2;
		}else{/*one digit number*/
			num_of_reg = operands[i] - ZERO;
			i++;
		}

		/*the register exist*/
		if(temp_reg == on && (num_of_reg > MAX_REGISTER || num_of_reg < MIN_REGISTER))
		{print_errors(REGISTER_DONT_EXIST, line_number, &none, operands);}
		
		switch(curr_opcode)
		{
			case OP_JMP:
				temp_address = (temp_reg == on)? num_of_reg: adress;
				break;

			case OP_LA:case OP_CALL:
				temp_address = adress;
				temp_reg = off;
				break;
	
			case OP_STOP:
				temp_address = off;
				temp_reg = off;
				break;
			
		}/*end of switch*/	

		OPERANDS_WHITE_SPACE_SKIP;
		
	}/*end of for loop*/
	

	/*adds the final values.*/
	curr.instruct.address = temp_address;
	curr.instruct.reg = temp_reg;
	return curr;
}/*END  create_J_instruction()*/

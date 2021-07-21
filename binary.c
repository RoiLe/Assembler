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
R create_R_instruction(char *, char *, int, int);




void instruction_binary_line(char *machineCode, char *key_word , char *operands)
{
	R curr_R_line;
	I curr_I_line;
	J curr_J_line;
	int curr_opcode = 0, curr_funct = 0;
	  
	int type_of_key_word = get_instruction_type(key_word, &curr_opcode, &curr_funct);
		
	if(!type_of_key_word){printf("ERROR: Does not recognize the keyword.\n");}
	
	switch(type_of_key_word)/*call to the correct func depened the type*/
	{
		case R_LINE_LOGIC: case R_LINE_COPY: 

			/*create here the struct of R instruction.. */
			curr_R_line = create_R_instruction(key_word, operands, curr_opcode, curr_funct);

			/*lets make the structure binary.. */
			int_to_binary(machineCode, curr_R_line.size,WORD);
			break;

		case I_LINE_LOGIC: case I_LINE_BRANCH: case I_LINE_SORE_LOAD:

			/*create_I_instruction(key_word, operands, type_of_key_word);*/
			curr_I_line = create_I_instruction(key_word, operands, type_of_key_word, curr_opcode, off);

			/*lets make the structure binary.. */
			int_to_binary(machineCode, curr_I_line.size,WORD);
			break;
		
		case J_LINE_SPECIAL:
			
			/*create_J_instruction(key_word, operands, type_of_key_word);*/
			curr_J_line = create_J_instruction(key_word, operands, curr_opcode, off);

			/*lets make the structure binary.. */
			int_to_binary(machineCode, curr_J_line.size,WORD);
			break;

		default:
			strcpy(machineCode, "will be create at the future. ");
	}/*end switch(type_of_key_word)*/



}/*END createInstruction()*/


int guidance_binary_lines(char *machine_code,char *key_word, int data)
{
	/*declarations*/
	DB_CODE curr_db_asciz;
	DH_CODE	curr_dh;
	DW_CODE curr_dw;

	int type_of_guidance = get_guidance_type(key_word);	

	switch(type_of_guidance)
	{ 
		case DB:
		case ASCIZ:		
			/*Adds the current data */		
			curr_db_asciz.guidance.byte = data;
			/*lets make the structure binary.. */
			int_to_binary(machine_code, curr_db_asciz.size,BYTE);
			break;
		case DH:
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

	}/*end of switch*/

	return type_of_guidance;
}/*END create_guidance_code()*/

void int_to_binary(char *machine_code, int curr, int num_of_byte)
{
	unsigned mask = 1;

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


R create_R_instruction(char *key_word, char *operands, int curr_opcode, int curr_funct)
{	
	/*declarations*/
	int i = 0 , comma_flag = off, 
	operands_counter = 0, temp_rd = 0, temp_rt = 0, temp_rs = 0,
	num_of_reg = -1, diff_to_int = 48;	
	R curr;


	/*add the correct funct and opcode. */
	curr.instruct.funct = curr_funct;
	curr.instruct.opcode = curr_opcode;

	
	/*add the registers. */
	for(i = 0; i < strlen(operands);)
	{		
		WHITE_SPACE_SKIP;
		
		if(operands[i] == '$' && comma_flag == off)
		{
			i++;
			comma_flag = on;
			operands_counter++;

			/*check if its two digit number.*/
			 if(operands[i + 1] >= 48 && operands[i + 1] <= 57)
			{
				num_of_reg = ((operands[i] - diff_to_int)*10) + (operands[i + 1] - diff_to_int);
				i = i+2;
			}else{
				num_of_reg = operands[i] - diff_to_int;
				i++;
			}

			if(num_of_reg >= 0 && num_of_reg <= 31)
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
						if(curr.instruct.opcode == OP_MOVE){printf("ERROR - its not the correct operator.\n");}
						else{temp_rd = num_of_reg;}	
						break;
					default:
						printf("ERROR - too many operands.\n");

				}/*end switch*/
				num_of_reg = -1;
			}
		}else{
			if(operands[i++] == ','){comma_flag = off;}
			else{printf("ERROR - dont exist register.\n");}
		}
	
		WHITE_SPACE_SKIP;
		
	
	}/*end for loop*/		

	/*adds the final values.*/
	curr.instruct.rd = temp_rd;
	curr.instruct.rt = temp_rt;
	curr.instruct.rs = temp_rs;

	/*always zero in this scenario. */
	curr.instruct.NONE = 0;	

	return curr;
}/*END create_R_instruction()*/


I create_I_instruction(char *key_word, char *operands, int type_of_I, int curr_opcode, long curr_immed)
{
	/*declarations*/
	I curr;
	int i, temp_rs = 0, temp_rt = 0, comma_flag = off, minus_flag = off,
	num_of_reg = 0, operands_counter = 0,  diff_to_int = 48;
	unsigned int temp_immed = 0;

	
	/*add the correct opcode. */
	curr.instruct.opcode = curr_opcode;

	/*add the operands. */
	for(i = 0; i < strlen(operands);)
	{
		WHITE_SPACE_SKIP;
		
		if(operands[i] == '$' && comma_flag == off)
		{
			i++;
			comma_flag = on;
			operands_counter++;

			/*check if its two digit number.*/
			 if(operands[i + 1] >= 48 && operands[i + 1] <= 57)
			{
				num_of_reg = ((operands[i] - diff_to_int)*10) + (operands[i + 1] - diff_to_int);
				i = i+2;
			}else{
				num_of_reg = operands[i] - diff_to_int;
				i++;
			}

			/*the register exist. */
			if(num_of_reg >= 0 && num_of_reg <= 31)
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
						printf("ERROR - too many operands.\n");

				}/*end switch*/
				num_of_reg = -1;
			}

		}else{
			if(operands[i] == ',')
			{
				comma_flag = off;
				i++;
			}
			else if(comma_flag == off)
			{
				while(operands[i] == '-' ||(operands[i] >= '0' && operands[i] <= '9'))
				{
					if(operands[i] == '-'){minus_flag = on;}
					else{temp_immed = operands[i] -	48 + (temp_immed * 10);}
					i++;
				}
				comma_flag = on;
				if(minus_flag == on){temp_immed = (-1) * temp_immed ;}			
			}		
			/*its a label*/
			else if((operands[i] >= 'A' && operands[i] <= 'Z') || (operands[i] >= 'a' && operands[i] <= 'z'))
			{
				temp_immed = curr_immed;
				i++;
			}else{				
				printf("ERROR - dont exist register.\n");
				i++;
			}			
		}
		WHITE_SPACE_SKIP;

	}/*end for loop*/

	/*adds the final values.*/
	curr.instruct.immed = temp_immed;	
	curr.instruct.rs = temp_rs;
	curr.instruct.rt = temp_rt;
	return curr;

}/*END create_I_instruction()*/

J create_J_instruction(char *key_word, char *operands, int curr_opcode, long adress)
{
	J curr;		
	int i, temp_reg = off, temp_address = 0, num_of_reg = 0;
	/*add the correct opcode. */
	curr.instruct.opcode = curr_opcode;

	/*add the operands. */
	for(i = 0; i < strlen(operands);)
	{
		WHITE_SPACE_SKIP;
		/*there is a regsiger.*/
		if(operands[i] == '$'){temp_reg = on;}
		i++;

		if(operands[i + 1] >= 48 && operands[i + 1] <= 57)
		{
			num_of_reg = ((operands[i] - 48)*10) + (operands[i + 1] - 48);
			i = i+2;
		}else{
			num_of_reg = operands[i] - 48;
			i++;
		}

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

		WHITE_SPACE_SKIP;

	}/*end of for loop*/

	/*adds the final values.*/
	curr.instruct.address = temp_address;
	curr.instruct.reg = temp_reg;
	return curr;
}/*END  create_J_instruction()*/

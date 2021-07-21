#ifndef ASSEMBLER_H
#define ASSEMBLER_H


 
/*
	The proccess of the two passes assembler.
	allocates memorys for the tables, than
	passes through the whole file and orgenaize
	the data.
	
	input -> declarations -> memory allocations -> first pass -> second pass -> output
	
	**the generals variables**
	ic -> Data image adress counter
	dc -> Data's counter
	guid_line_count -> Counter of guidances data lines.
	assemblyCode -> The assembly code (the input.)
	exeptions -> If there exeptions: false else: true
	currentNumberLine -> The number of the current line.
	currLine -> The current line.
	label -> If there is a label it's here.
	key_word -> The word that defines the action of the line
	operands -> The operands of the current line.
	typeOfSentence -> The type of the sentences [EMPTY_LINE,NOTE_LINE,INSTRUCTION_LINE,GUIDANCE_LINE ]
	temp_curr_line -> temporary current line.
	curr_adress -> The current adress. 
	
	**The data image variables**
	instruction_head 
	instruction_tail
	p_instruction -> instruction pointer


	guidance_head
	guidance_tail
	p_guidance -> guidances pointer

	**The symbol table variables**
	symbol_table_head
	symbol_table_tail 
	p_symbol_table -> symol table pointer.
	 
*/
int proccess_program(char*); 

#endif

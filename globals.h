#define IC 100 
#define DC 0	  
#define NEXT_ADRESS 4
#define LINE_MAX_LENGTH 80 
#define MAX_ARRTIBUTE_LENGTH 15 
#define KEY_WORD_MAX_LENGTH 10  
#define LABEL_MAX_LENGTH 31  
#define MACHINE_CODE_LENGTH 40 
#define MAX_REGISTER 31  
#define MIN_REGISTER 0  
    
                              
enum file_succeeded {  
	SUCCESSED = 0, FAILED = 1    
}; 
  
enum flags { 
	off = 0, on = 1
};   
 
typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool; 
   
    
typedef enum type_of_instruction{

	R_LINE_LOGIC = 1,
	R_LINE_COPY, 
	
	I_LINE_LOGIC,
	I_LINE_BRANCH,
	I_LINE_SORE_LOAD, 

	J_LINE_SPECIAL

}type_ins;
       
typedef enum type_of_guidance{ 
  
	DB = 1,
	DH,
	DW,
	ASCIZ,
	ENTRY,
	EXTERN

}type_guide;

typedef enum funct{

	/*R arithmetic instructions*/
	FU_ADD = 1,
	FU_SUB,
	FU_AND,
	FU_OR,
	FU_NOR,

	/*R copy instructions*/
	FU_MOVE = 1,
	FU_MVHI,
	FU_MVLO

}funct;

typedef enum opcodes{

	/*R opcodes*/
	OP_ADD = 0,
	OP_SUB = 0,
	OP_AND = 0,
	OP_OR = 0,
	OP_NOR = 0,
	OP_MOVE = 1,
	OP_MVHI = 1,
	OP_MVLO = 1,

	/*I opcodes*/
	OP_ADDI = 10,
	OP_SUBI,
	OP_ANDI,
	OP_ORI,
	OP_NORI,
	OP_BNE,
	OP_BEQ,
	OP_BLT,
	OP_BGT,
	OP_LB,
	OP_SB, 
	OP_LW,
	OP_SW,
	OP_LH,
	OP_SH,

	/*J opcodes*/ 
	OP_JMP = 30, 
	OP_LA, 
	OP_CALL ,  
	OP_STOP = 63  
}opcode;   
 

enum size_of_byte{
	BYTE = 8,
	HALF_WORD = 16, 
	WORD = 32 	 
}; 
 
typedef struct symbol_line{ 
	char symbol[LABEL_MAX_LENGTH];
	long value; 
	char attribute[MAX_ARRTIBUTE_LENGTH];      
	struct symbol_line *next;
}symLine;

typedef struct dataImage *DI_ptr;  
typedef struct dataImage{  
	long adress;
	char sourceCode[LINE_MAX_LENGTH]; 
	char machineCode[MACHINE_CODE_LENGTH]; 
	DI_ptr next;
}dataImg;


typedef union instructionR{
	struct instruction_R_line{
		unsigned int NONE: 6;
		unsigned int funct: 5;
		unsigned int rd: 5;
		unsigned int rt: 5;	
		unsigned int rs: 5;
		unsigned int opcode: 6;
	}instruct;
	int size: 32;
}R;

typedef union instructionI{
	struct instruction_I_line{
		unsigned int immed: 16;
		unsigned int rt: 5;	
		unsigned int rs: 5;
		unsigned int opcode: 6;
	}instruct;
	int size: 32;
}I;

typedef union instructionJ{
	struct instruction_J_line{
		unsigned int address: 25;	
		unsigned int reg: 1;
		unsigned int opcode: 6;
	}instruct;
	int size: 32;
}J;


typedef union guidanceDB{
	struct guidance_byte{
		unsigned int byte: 8;	
	}guidance;
	int size: 8;
}DB_CODE;

typedef union guidanceDH{
	struct guidance_half{
		unsigned int half_word: 16;	
	}guidance;
	int size: 16;
}DH_CODE;

typedef union guidanceDW{
	struct guidance_word{
		unsigned int word: 32;	
	}guidance;
	int size: 32;
}DW_CODE;


/*- קובץ header הצהרות על פונקציות שמשתמשים בהם במהלך כל התוכנית (registers,opcode,instructions, IC&DC, symbol-table ודומיהם)*/
#define IC 100
#define DC 0	
#define NEXT_ADRESS 4
#define LINE_MAX_LENGTH 80
#define LABEL_MAX_LENGTH 31

enum file_succeeded {
	SUCCESSED = 0, FAILED = 1
};

enum flags {
	off = 0, on = 1
};

typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool;

typedef enum arrtibutes {
	CODE = 0,
	DATA , 
	ENTRY ,
	EXTERNAL
}arr;

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

/*typedef enum registers {
	$0 = 0,
	$1,
	$2,
	$3,
	$4,
	$5,
	$6,
	$7,
	$8,
	$9,
	$10,
	$11,
	$12,
	$13,
	$14,
	$15,
	$16,
	$17,
	$18,
	$19,
	$20,
	$21,
	$22,
	$23,
	$24,
	$25,
	$26,
	$27,
	$28,
	$29,
	$30,
	$31

} reg;*/


typedef struct symbol_line{
	char symbol[LABEL_MAX_LENGTH];
	long value;
	char *attribute;
	struct symbol_line *next;
}symLine;

typedef struct dataImage *DI_ptr;
typedef struct dataImage{
	long adress;
	char sourceCode[LINE_MAX_LENGTH];
	char machineCode[32];
	DI_ptr next;
}dataImg;


union instructionR{
	struct instruction_R_line{
		unsigned int NONE: 5;
		unsigned int funct: 5;
		unsigned int rd: 5;
		unsigned int rt: 5;	
		unsigned int rs: 5;
		unsigned int opcode: 6;
	}instruct;
	int size: 31;
}R;

union instructionI{
	struct instruction_I_line{
		unsigned int immed: 15;
		unsigned int rt: 5;	
		unsigned int rs: 5;
		unsigned int opcode: 6;
	}instruct;
	int size: 31;
}I;

union instructionJ{
	struct instruction_J_line{
		unsigned int address: 24;	
		unsigned int reg: 1;
		unsigned int opcode: 6;
	}instruct;
	int size: 31;
}J;







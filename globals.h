/*- קובץ header הצהרות על פונקציות שמשתמשים בהם במהלך כל התוכנית (registers,opcode,instructions, IC&DC, symbol-table ודומיהם)*/
#define IC 100
#define DC 0	
#define NEXT_ADRESS
#define LINE_MAX_LENGTH 80


typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool;

typedef enum errors{
	INPUT_ERROR = -1,
	CORRECT = 0,
	NAME_ERROR = 1,
	DONT_EXIST_FILE_ERROR = 2, 
	TOO_LONG_LINE_COMMAND_ERROR = 3
}err;

enum TypeOfSentence{
	EMPTY_LINE = 1,
	NOTE_LINE,
	INSTRUCTION_LINE,
	GUIDANCE_LINE 
};

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


typedef struct symboleTable{
	char * symbol;
	int value;
	char *attribute;

}symTab;

typedef struct dataImage{
	int *adress;
	char *sourceCode;
	char machineCode[32];
	struct dataImage *nextAdress;
}dataImg;

typedef struct instructionR{
	unsigned int NONE: 5;
	unsigned int funct: 5;
	unsigned int rd: 5;
	unsigned int rt: 5;	
	unsigned int rs: 5;
	unsigned int opcode: 6;
}R;


typedef struct instructionI{
	unsigned int immed: 15;
	unsigned int rt: 5;	
	unsigned int rs: 5;
	unsigned int opcode: 6;
}I;

typedef struct instructionJ{
	unsigned int address: 24;	
	unsigned int reg: 1;
	unsigned int opcode: 6;
}J;







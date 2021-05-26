/*- קובץ header הצהרות על פונקציות שמשתמשים בהם במהלך כל התוכנית (registers,opcode,instructions, IC&DC, symbol-table ודומיהם)*/
#define IC 100
#define DC 0

typedef enum success{
	failed = 0, succesed = 1
}suc;

typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool;

typedef enum errors{
	NAME_ERROR = 1;



}err;

typedef enum registers {
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
	$31,

} reg;


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

typedef struct symboleTable{
	char * symbol;
	int value;
	char *attribute;
}symTab;





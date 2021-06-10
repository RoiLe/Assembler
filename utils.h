#ifndef UTILS_H
#define UTILS_H

enum TypeOfSentence{
	EMPTY_LINE = 1,
	NOTE_LINE,
	INSTRUCTION_LINE,
	GUIDANCE_LINE 
};

/*
	checks what the type of the sentense.
	return the type. 
*/
int classification_of_sentence(char*);


/*
	find if there is a label in the line.
	return the label or NULL.
*/
char *find_the_label(char *);

/*


*/
char *revese_string(char*);

#endif

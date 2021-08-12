;file ps.as
;sample source code

	.entry RL
MAIN:	add $3,$5,$9
	ori $9,-5,$2
	call RL
 	la val1
	or $12,$8,$10
 	jmp Next
	move $20,$4
 	la K
RL:	ori $20,-56,$17
 	sw $0,4,$10
	mvlo $25,$11
	bgt $27,$22,RL
 	call val1
K: 	jmp $4
	sh $30,217,$14
	jmp $19
END:	stop

	.asciz "Roi Levi"
 	.db 6,-9,15,-3
 	.dh 4598,12541
	.entry K
	.dw 15
	.extern val1 

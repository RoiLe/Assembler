
;file errors1.as
;sample first pass erorrs source code 

;error 1 - syntax
MAIN: add $3,$5,$9,

;error 2 - number of operands
LOOP: ori $30,-5,$2,$20

 la val1
 jmp Next

;error 3 - dont exist register 
Next: move $20,$40

 bgt $4,$2,END
 la K

;error 4 - number of operands
 sw 4,$10

 bne $31,$9, LOOP
 call val1
 jmp $4

;error 5- label redifined
LOOP: stop

;error 6 - syntax
STR: .asciz "aBcd

;error 7 - guidance line syntax error
LIST: .dvb 6,-9

;error 8 - exceptional numeric value
 .dh 32768

.entry K
K: .dw 31,-12
.extern val1 


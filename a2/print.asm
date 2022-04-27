print:
sw $1, -4($30) ; push $1
sw $4, -8($30) ; push $4
sw $5, -12($30) ; push $5
sw $6, -16($30) ; push $6
sw $7, -20($30) ; push $7
sw $9, -24($30) ; push $9
sw $10, -28($30) ; push $10
sw $11, -32($30) ; push $11
sw $12, -36($30) ; push $12
sw $13, -40($30) ; push $13
sw $14, -44($30) ; push $14
sw $15, -48 ($30) ; push $15
sw $16, -52($30) ; push $16
lis $4
.word 52
sub $30, $30, $4 ; update stack pointer


lis $10
.word 10
lis $11
.word 0
lis $16
.word 1
lis $13
.word 45
lis $6
.word 0xffff000c
lis $7
.word 48
lis $14
.word 10

slt $12, $1,$11
bne $12, $16, 2
sw $13, 0($6)
sub $1, $11, $1 ;set the $1 to its absolute value

slt $15, $1, $10
beq $15, $16, 13 ;go to final part is |$1| < only has one digit

slt $15, $14, $1
beq $15, $11, 3
mult $14, $10
mflo $14
beq $15, $15, -5 ;find the smallest 10^n number that is larger than $1

div $14, $10
mflo $14
div $1, $14
mflo $4
mfhi $5
add $9, $4, $7
sw $9, 0($6)
add $1, $5, $11
div $14, $10
mflo $14
bne $16, $14, -9 ;print out the all the digit of $1 (except the last digit)

add $9, $1, $7
sw $9, 0($6)
sw $10, 0($6) ;print the last digit and the newline

;epilogue
lis $4 
.word 52
add $30, $30, $4 ; update stack pointer
lw $1, -4($30) ; pop $1
lw $4, -8($30) ; pop $4
lw $5, -12($30) ; pop $5
lw $6, -16($30) ; pop $6
lw $7, -20($30) ; pop $7
lw $9, -24($30) ; pop $9
lw $10, -28($30) ; pop $10
lw $11, -32($30) ; pop $11
lw $12, -36($30) ; pop $12
lw $13, -40($30) ; pop $13
lw $14, -44($30) ; pop $14
lw $15, -48 ($30) ; pop $15
lw $16, -52($30) ; pop $16

jr $31


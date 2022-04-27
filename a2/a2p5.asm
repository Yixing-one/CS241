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

slt $12, $1, $11
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
mflo $
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

jr $31


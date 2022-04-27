lis $4
.word 4
lw $3, 0($1)
lis $5
.word 0
lis $6
.word 0
lis $7
.word 1
add $8, $1, $5
add $6, $6, $7
beq $6, $2, 6
add $8, $8, $4
lw $9, 0($8)
slt $10, $3, $9
beq $10, $5, 1
add $3, $9, $5
beq $5, $5, -8
jr $31


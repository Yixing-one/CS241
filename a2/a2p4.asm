lis $4
.word 4
lis $5
.word 0xffff000c
lis $6
.word 0
lis $10
.word 0
lis $11
.word 1
lis $12
.word 32
lis $13
.word 64
add $7, $1, $10
beq $2, $10, 10
beq $6, $2, 9
add $6, $6, $11
lw $8, 0($7)
bne $8, $10, 1
sw $12, 0($5)
beq $8, $10, 2
add $14, $8, $13
sw $14, 0($5)
add $7, $7, $4
beq $6, $6, -10
jr $31


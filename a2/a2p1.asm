slt $4, $1, $2
lis $5
.word 0
add $3, $5, $1
beq $4, $5, 1
add $3, $5, $2
jr $31


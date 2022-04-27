; Main prologue
.import print
lis $10
.word print
.import new
.import delete
lis $4
.word 4
lis $11
.word 1
sub $29, $30, $4
sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lis $5
add $30, $30, $4
; End prologue
lis $3
.word 0
sw $3, -8($29)
sub $30, $30, $4
lis $3
.word 2
sw $3, -12($29)
sub $30, $30, $4
lw $3, 0($29)
sw $3, -8($29)
whileloop0:
lis $3
.word 0
add $6, $3, $0
lw $3, -12($29)
slt $3, $6, $3
beq $3, $0, done0
lw $3, -8($29)
sw $3, 0($29)
whileloop1:
lis $3
.word 0
add $6, $3, $0
lw $3, 0($29)
slt $3, $6, $3
beq $3, $0, done1
lw $3, 0($29)
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
jalr $10
lw $31, 0($30)
add $30, $30, $4
lw $3, 0($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, -4($29)
lw $5, 0($30)
add $30, $30, $4
sub $3, $5, $3
sw $3, 0($29)
beq $0, $0, whileloop1
done1:
lis $3
.word 0
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
jalr $10
lw $31, 0($30)
add $30, $30, $4
lw $3, -12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
lw $5, 0($30)
add $30, $30, $4
sub $3, $5, $3
sw $3, -12($29)
beq $0, $0, whileloop0
done0:
lw $3, 0($29)
; Main epilogue
add $30, $29, $4
jr $31

; import useful function
.import print
.import init
.import new
.import delete
lis $11
.word 1
lis $4
.word 4
sub $29, $30, $4
sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
add $2, $0, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word init
jalr $5
lw $31, 0($30)
add $30, $30, $4
; End function set
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lis $3
.word 5
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ffact
jalr $5
lis $5
.word 4
add $30, $30, $5
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
add $30, $29, $4
jr $31
Ffact:
sub $29, $30, $4
lis $3
.word 0
sw $3, 0($29)
sub $30, $30, $4
sw $1, -4($30)
sub $30, $30, $4
sw $2, -4($30)
sub $30, $30, $4
sw $5, -4($30)
sub $30, $30, $4
sw $6, -4($30)
sub $30, $30, $4
sw $7, -4($30)
sub $30, $30, $4
sw $8, -4($30)
sub $30, $30, $4
lw $3, 4($29)
add $6, $3, $0
lis $3
.word 0
slt $7, $6, $3
slt $8, $3, $6
add $3, $7, $8
sub $3, $11, $3
bne $3, $11, else0
lis $3
.word 1
sw $3, 0($29)
beq $0, $0, endif0
else0:
lw $3, 4($29)
sw $3, -4($30)
sub $30, $30, $4
sw $29, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 4($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
lw $5, 0($30)
add $30, $30, $4
sub $3, $5, $3
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word Ffact
jalr $5
lis $5
.word 4
add $30, $30, $5
lw $31, 0($30)
add $30, $30, $4
lw $29, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
mult $5, $3
mflo $3
sw $3, 0($29)
endif0:
lw $3, 0($29)
lw $8, 0($30)
add $30, $30, $4
lw $7, 0($30)
add $30, $30, $4
lw $6, 0($30)
add $30, $30, $4
lw $5, 0($30)
add $30, $30, $4
lw $2, 0($30)
add $30, $30, $4
lw $1, 0($30)
add $30, $30, $4
add $30, $29, $4
jr $31

lis $10
.word 0
lis $11
.word 1
lis $12
.word -1
lis $4 
.word 4
lis $3
.word 0
lis $5
.word 0


sw $31, -4($30) ; push $31
lis $24
.word 4
sub $30, $30, $24

lis $20
.word length
jalr $20

lis $24
.word 4
add $30, $30, $24
lw $31, -4($30) ; pop $31

jr $31


length:
add $3, $3, $11
add $28, $10, $10
add $29, $10, $10

; check left node
add $7, $5, $11
mult $7, $4
mflo $24
add $24, $24, $1
lw $8, 0($24)
beq $8, $12, 20

sw $5, -4($30) ; push $5
sw $3, -8($30) ; push $3
sw $27, -12($30) ; push $27
sw $29, -16($30) ; push $29
sw $31, -20($30) ; push $31
lis $24
.word 20
sub $30, $30, $24

lis $27
.word childtree
jalr $27

add $28, $3, $10
lis $24
.word 20
add $30, $30, $24
lw $5, -4($30) ; pop $5
lw $3, -8($30) ; pop $3
lw $27, -12($30); pop $27
lw $29, -16($30) ; pop $29
lw $31, -20($30) ; pop $31

; check right node
add $7, $5, $11
add $7, $7, $11
mult $7, $4
mflo $24
add $24, $24, $1
lw $8, 0($24)
beq $8, $12, 20

sw $5, -4($30) ; push $5
sw $3, -8($30) ; push $3
sw $27, -12($30) ; push $27
sw $28, -16($30) ; push $29
sw $31, -20($30) ; push $31
lis $24
.word 20
sub $30, $30, $24

lis $26
.word childtree
jalr $26

add $29, $3, $10
lis $24
.word 20
add $30, $30, $24
lw $5, -4($30) ; pop $5
lw $3, -8($30) ; pop $3
lw $27, -12($30); pop $27
lw $28, -16($30) ; pop $29
lw $31, -20($30) ; pop $31


; update height
slt $25, $28, $29
beq $25, $10, 1
add $3, $3, $29
beq $25, $11, 1
add $3, $3, $28
jr $31

childtree:
add $5, $8, $10  ;$5 = left node value
add $3, $10, $10 ;$3 = 0

sw $31, -4($30) ; push $31
lis $24
.word 4
sub $30, $30, $24

lis $27
.word length
jalr $27

lis $24
.word 4
add $30, $30, $24
lw $31, -4($30) ; pop $31

jr $31


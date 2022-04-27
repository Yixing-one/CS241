stirling:
; n = $1 and k = $2, and stores the value for f(n,k) in $3

; $1 the value of n
; $2 the value of k
; $3 result (not preserved)
; $6 is a temporary register that store the value of (n-1) * f(n-1,k)
; $12 is a temporary register that store the value of f(n-1,k)
; $7 is a temporary register that store the value of f(n-1,k-1)

; push all registers we change 
sw $1, -4($30)
sw $2, -8($30)
sw $5, -12($30)
sw $6, -16($30)
sw $10, -20($30)
sw $11, -24($30)
sw $4, -28($30)
sw $31, -32($30)
sw $12, -36($30)
sw $7, -40($30)


lis $4
.word 40
sub $30, $30, $4

lis $10
.word 0
lis $11
.word 1

bne $1, $2, 3	; check for f(0,0)
bne $1, $10, 2
add $3, $11, $10
beq $1, $1, end 

add $3, $10, $10	 ; check for f(i,0), f(0,i)
beq $1, $10, end 
beq $2, $10, end 

sub $1, $1, $11 ; calculate (n-1) * f(n-1,k) and store in $6
lis $5
.word stirling
jalr $5
add $12, $10, $3
multu $12, $1
mflo $6


sub $2, $2, $11 ; calculate f(n-1,k-1) and store in $7
lis $5
.word stirling
jalr $5
add $7, $3, $10

add $3, $6, $7 ; set $3 to be (n-1) * f(n-1,k) + f(n-1,k-1

end:
; pop all registers we pushed 
lis $4
.word 40
add $30, $30, $4
lw $1, -4($30)
lw $2, -8($30)
lw $5, -12($30)
lw $6, -16($30)
lw $10, -20($30)
lw $11, -24($30)
lw $4, -28($30)
lw $31, -32($30)
lw $12, -36($30)
lw $7, -40($30)
jr $31


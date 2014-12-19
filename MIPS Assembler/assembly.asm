addi $s1,$s0,0
addi $s2,$s0,10

l1:
add $s3,$s3,$s1
addi $s1,$s1,1

slt $t1,$s2,$s1
beq  $t1,$0,l1
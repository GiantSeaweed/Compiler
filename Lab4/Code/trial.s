.data
__prompt: .asciiz "Enter an integer:"
__ret: .asciiz "\n"
.globl main
.text
f:
lw $t1, 12($sp)
sw $t1, 0($sp)
lw $t1, -4($sp)
lw $t2, 0($sp)
move $t1, $t2
sw $t1, -4($sp)
lw $t1,-4($sp)
move $v0, $t1
jr $ra
main:
lw $t1, -12($sp)
li $t2, 0
move $t1, $t2
sw $t1, -12($sp)
lw $t1, -16($sp)
lw $t2, -12($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -16($sp)
addi $t1, $sp,-12
sw $t1, -20($sp)
lw $t1, -24($sp)
lw $t2, -20($sp)
lw $t3, -16($sp)
add $t1, $t2, $t3
sw $t1, -24($sp)
li $v0, 4
la $a0, __prompt
syscall
li $v0, 5
syscall
sw $v0, -28($sp)
lw $t1,-28($sp)
lw $t2,-24($sp)
sw $t1, 0($t2)
lw $t1,-24($sp)
lw $t2,-4($sp)
lw $t2, 0($t1)
sw $t2, -4($sp)
lw $t1, -32($sp)
li $t2, 1
move $t1, $t2
sw $t1, -32($sp)
lw $t1, -36($sp)
lw $t2, -32($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -36($sp)
addi $t1, $sp,-12
sw $t1, -40($sp)
lw $t1, -44($sp)
lw $t2, -40($sp)
lw $t3, -36($sp)
add $t1, $t2, $t3
sw $t1, -44($sp)
li $v0, 4
la $a0, __prompt
syscall
li $v0, 5
syscall
sw $v0, -48($sp)
lw $t1,-48($sp)
lw $t2,-44($sp)
sw $t1, 0($t2)
lw $t1,-44($sp)
lw $t2,-28($sp)
lw $t2, 0($t1)
sw $t2, -28($sp)
lw $t1, -52($sp)
li $t2, 0
move $t1, $t2
sw $t1, -52($sp)
lw $t1, -56($sp)
lw $t2, -52($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -56($sp)
addi $t1, $sp,-12
sw $t1, -60($sp)
lw $t1, -64($sp)
lw $t2, -60($sp)
lw $t3, -56($sp)
add $t1, $t2, $t3
sw $t1, -64($sp)
lw $t1,-64($sp)
lw $t2,-68($sp)
lw $t2, 0($t1)
sw $t2, -68($sp)
lw $t1, -72($sp)
li $t2, 1
move $t1, $t2
sw $t1, -72($sp)
lw $t1, -76($sp)
lw $t2, -72($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -76($sp)
addi $t1, $sp,-12
sw $t1, -80($sp)
lw $t1, -84($sp)
lw $t2, -80($sp)
lw $t3, -76($sp)
add $t1, $t2, $t3
sw $t1, -84($sp)
lw $t1,-84($sp)
lw $t2,-88($sp)
lw $t2, 0($t1)
sw $t2, -88($sp)
lw $t1,-68($sp)
lw $t2,-88($sp)
blt $t1, $t2, label1
j label2
label1:
lw $t1, -92($sp)
li $t2, 1
move $t1, $t2
sw $t1, -92($sp)
lw $t1, -96($sp)
li $t2, 0
lw $t3, -92($sp)
sub $t1, $t2, $t3
sw $t1, -96($sp)
li $v0, 1
lw $a0, -96($sp)
syscall
li $v0, 4
la $a0, __ret
syscall
j label3
label2:
lw $t1, -100($sp)
li $t2, 0
move $t1, $t2
sw $t1, -100($sp)
lw $t1, -104($sp)
lw $t2, -100($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -104($sp)
addi $t1, $sp,-12
sw $t1, -108($sp)
lw $t1, -112($sp)
lw $t2, -108($sp)
lw $t3, -104($sp)
add $t1, $t2, $t3
sw $t1, -112($sp)
lw $t1,-112($sp)
lw $t2,-116($sp)
lw $t2, 0($t1)
sw $t2, -116($sp)
lw $t1, -120($sp)
li $t2, 1
move $t1, $t2
sw $t1, -120($sp)
lw $t1, -124($sp)
lw $t2, -120($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -124($sp)
addi $t1, $sp,-12
sw $t1, -128($sp)
lw $t1, -132($sp)
lw $t2, -128($sp)
lw $t3, -124($sp)
add $t1, $t2, $t3
sw $t1, -132($sp)
lw $t1,-132($sp)
lw $t2,-136($sp)
lw $t2, 0($t1)
sw $t2, -136($sp)
lw $t1,-116($sp)
lw $t2,-136($sp)
ble $t1, $t2, label4
j label5
label4:
lw $t1, -140($sp)
li $t2, 0
move $t1, $t2
sw $t1, -140($sp)
li $v0, 1
lw $a0, -140($sp)
syscall
li $v0, 4
la $a0, __ret
syscall
label5:
label3:
lw $t1, -144($sp)
li $t2, 0
move $t1, $t2
sw $t1, -144($sp)
lw $t1, -148($sp)
lw $t2, -144($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -148($sp)
addi $t1, $sp,-12
sw $t1, -152($sp)
lw $t1, -156($sp)
lw $t2, -152($sp)
lw $t3, -148($sp)
add $t1, $t2, $t3
sw $t1, -156($sp)
lw $t1,-156($sp)
lw $t2,-160($sp)
lw $t2, 0($t1)
sw $t2, -160($sp)
lw $t1, -164($sp)
li $t2, 1
move $t1, $t2
sw $t1, -164($sp)
lw $t1, -168($sp)
lw $t2, -164($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -168($sp)
addi $t1, $sp,-12
sw $t1, -172($sp)
lw $t1, -176($sp)
lw $t2, -172($sp)
lw $t3, -168($sp)
add $t1, $t2, $t3
sw $t1, -176($sp)
lw $t1,-176($sp)
lw $t2,-180($sp)
lw $t2, 0($t1)
sw $t2, -180($sp)
lw $t1,-160($sp)
lw $t2,-180($sp)
bgt $t1, $t2, label6
j label7
label6:
lw $t1, -184($sp)
li $t2, 1
move $t1, $t2
sw $t1, -184($sp)
li $v0, 1
lw $a0, -184($sp)
syscall
li $v0, 4
la $a0, __ret
syscall
j label8
label7:
lw $t1, -188($sp)
li $t2, 0
move $t1, $t2
sw $t1, -188($sp)
lw $t1, -192($sp)
lw $t2, -188($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -192($sp)
addi $t1, $sp,-12
sw $t1, -196($sp)
lw $t1, -200($sp)
lw $t2, -196($sp)
lw $t3, -192($sp)
add $t1, $t2, $t3
sw $t1, -200($sp)
lw $t1,-200($sp)
lw $t2,-204($sp)
lw $t2, 0($t1)
sw $t2, -204($sp)
lw $t1, -208($sp)
li $t2, 1
move $t1, $t2
sw $t1, -208($sp)
lw $t1, -212($sp)
lw $t2, -208($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -212($sp)
addi $t1, $sp,-12
sw $t1, -216($sp)
lw $t1, -220($sp)
lw $t2, -216($sp)
lw $t3, -212($sp)
add $t1, $t2, $t3
sw $t1, -220($sp)
lw $t1,-220($sp)
lw $t2,-224($sp)
lw $t2, 0($t1)
sw $t2, -224($sp)
lw $t1,-204($sp)
lw $t2,-224($sp)
bge $t1, $t2, label9
j label10
label9:
lw $t1, -228($sp)
li $t2, 0
move $t1, $t2
sw $t1, -228($sp)
li $v0, 1
lw $a0, -228($sp)
syscall
li $v0, 4
la $a0, __ret
syscall
label10:
label8:
lw $t1, -232($sp)
li $t2, 0
move $t1, $t2
sw $t1, -232($sp)
lw $t1, -236($sp)
lw $t2, -232($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -236($sp)
addi $t1, $sp,-12
sw $t1, -240($sp)
lw $t1, -244($sp)
lw $t2, -240($sp)
lw $t3, -236($sp)
add $t1, $t2, $t3
sw $t1, -244($sp)
lw $t1,-244($sp)
lw $t2,-248($sp)
lw $t2, 0($t1)
sw $t2, -248($sp)
lw $t1, -252($sp)
li $t2, 1
move $t1, $t2
sw $t1, -252($sp)
lw $t1, -256($sp)
lw $t2, -252($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -256($sp)
addi $t1, $sp,-12
sw $t1, -260($sp)
lw $t1, -264($sp)
lw $t2, -260($sp)
lw $t3, -256($sp)
add $t1, $t2, $t3
sw $t1, -264($sp)
lw $t1,-264($sp)
lw $t2,-268($sp)
lw $t2, 0($t1)
sw $t2, -268($sp)
lw $t1,-248($sp)
lw $t2,-268($sp)
beq $t1, $t2, label11
j label12
label11:
lw $t1, -272($sp)
li $t2, 0
move $t1, $t2
sw $t1, -272($sp)
li $v0, 1
lw $a0, -272($sp)
syscall
li $v0, 4
la $a0, __ret
syscall
label12:
lw $t1, -276($sp)
li $t2, 0
move $t1, $t2
sw $t1, -276($sp)
lw $t1, -280($sp)
lw $t2, -276($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -280($sp)
addi $t1, $sp,-12
sw $t1, -284($sp)
lw $t1, -288($sp)
lw $t2, -284($sp)
lw $t3, -280($sp)
add $t1, $t2, $t3
sw $t1, -288($sp)
lw $t1,-288($sp)
lw $t2,-292($sp)
lw $t2, 0($t1)
sw $t2, -292($sp)
lw $t1, -296($sp)
li $t2, 1
move $t1, $t2
sw $t1, -296($sp)
lw $t1, -300($sp)
lw $t2, -296($sp)
li $t3, 4
mul $t1, $t2, $t3
sw $t1, -300($sp)
addi $t1, $sp,-12
sw $t1, -304($sp)
lw $t1, -308($sp)
lw $t2, -304($sp)
lw $t3, -300($sp)
add $t1, $t2, $t3
sw $t1, -308($sp)
lw $t1,-308($sp)
lw $t2,-312($sp)
lw $t2, 0($t1)
sw $t2, -312($sp)
lw $t1,-292($sp)
lw $t2,-312($sp)
bne $t1, $t2, label13
j label14
label13:
lw $t1, -316($sp)
li $t2, 2333
move $t1, $t2
sw $t1, -316($sp)
li $v0, 1
lw $a0, -316($sp)
syscall
li $v0, 4
la $a0, __ret
syscall
label14:
lw $t1, -320($sp)
li $t2, 0
move $t1, $t2
sw $t1, -320($sp)
lw $t1,-320($sp)
move $v0, $t1
jr $ra

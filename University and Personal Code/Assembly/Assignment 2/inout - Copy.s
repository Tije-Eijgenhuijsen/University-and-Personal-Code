.text
assignment: .asciz "Assignment 2: inout\n"	#String with assignment name
formatstr: .asciz "%ld"						#String for scanf
output: .asciz "The new number is %d\n"

.global main

main:
		pushq	%rbp
		movq	%rsp, %rbp

		movq	$0, %rax			#No vector registers in use for printf
		movq	$assignment, %rdi	#Loading the adress of assignment
		call	printf				#Calling the printf routine
		
		call inout
		
end:
		mov		$0, %rdi			#Loading the program exit code
		call	exit				#exiting the program
		
inout:
		pushq	%rbp
		movq	%rsp, %rbp
		
		subq	$16, %rsp
		leaq	-16(%rbp), %rsi
		movq	$formatstr, %rdi
		movq	$0, %rax
		call	scanf
		
		movq	$0, %rax
		movq	%rsi, %rsi
		movq	$output, %rdi
		call	printf

		movq	%rbp, %rsp
		popq	%rbp
		ret
		
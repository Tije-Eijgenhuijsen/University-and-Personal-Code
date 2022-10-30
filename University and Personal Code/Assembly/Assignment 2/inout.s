.text
assignment: .asciz "Assignment 2: inout\n"	#String with assignment name
formatstr: .asciz "%ld"						#String for scanf
output: .asciz "The new number is %d\n"		#String for output

.global main

main:
		pushq	%rbp				#Initializing the base pointer
		movq	%rsp, %rbp

		movq	$0, %rax			#No vector registers in use for printf
		movq	$assignment, %rdi	#Loading the adress of assignment
		call	printf				#Calling the printf routine
		
		call 	inout				#Call inout function
		
end:
		mov		$0, %rdi			#Loading the program exit code
		call	exit				#exiting the program
		
inout:
		pushq	%rbp				#Prologue: pushes old base pointer
		movq	%rsp, %rbp			#sets new base pointer to current stack pointer
		
		subq	$16, %rsp			#Reserves space for local variable
		leaq	-16(%rbp), %rsi		#Loads adress of local variable to RSI
		movq	$formatstr, %rdi	#Loads first argument of scanf
		movq	$0, %rax			#No vector registers for scanf
		call	scanf				#Calls scanf
		
		incq	-16(%rbp)			#Increments the value of the local variable at said adress
		
		movq	$0, %rax			#No vector registers for printf
		movq	-16(%rbp), %rsi		#Loads value of local variable
		movq	$output, %rdi		#Loads the string adress
		call	printf				#Calls printf

		movq	%rbp, %rsp			#Epilogue: Clear local variables from the stack
		popq	%rbp				#Restore caller's base pointer
		ret							#Return from subroutine
		
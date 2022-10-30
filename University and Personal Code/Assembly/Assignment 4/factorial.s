.text
assignment: .asciz "Assignment 4: recursion\n"	#String with assignment name
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
		call	exit				#Exiting the program
		
inout:
		pushq	%rbp				#Prologue: pushes old base pointer
		movq	%rsp, %rbp			#Sets new base pointer to current stack pointer
		
		subq	$16, %rsp			#Reserves space for local variable
		leaq	-16(%rbp), %rsi		#Loads adress of local variable to RSI
		movq	$formatstr, %rdi	#Loads first argument of scanf
		movq	$0, %rax			#No vector registers for scanf
		call	scanf				#Calls scanf
		
		movq	-16(%rbp), %rdi		#Load first argument
		call	factorial			#Call factorial function
		
		movq	%rax, %rsi			#Loads value of local variable
		movq	$0, %rax			#No vector registers for printf
		movq	$output, %rdi		#Loads the string adress
		call	printf				#Calls printf

		movq	%rbp, %rsp			#Epilogue: Clear local variables from the stack
		popq	%rbp				#Restore caller's base pointer
		ret							#Return from subroutine
		
factorial:
		pushq	%rbp				#Prologue: pushes old base pointer
		movq	%rsp, %rbp			#Sets new base pointer to current stack pointer
		
		subq	$16, %rsp			#Reserves space for local variable
		movq	%rdi, -16(%rbp)		#Loads parametet into local variable
		
		cmpq	$1, -16(%rbp)		#Compares local variable to the value 1
		jle		endrecursion		#If local variable <= 1 then jump to endrecursion
				
		movq	-16(%rbp), %rdi		#Load first argument to %rdi
		dec		%rdi				#Decrement argument
		call	factorial			#Calls factorial
		
		mulq	-16(%rbp)			#Multiply RAX by local variable and store in RAX
				
		movq	%rbp, %rsp			#Epilogue: Clear local variables from the stack
		popq	%rbp				#Restore caller's base pointer
		ret							#Return from subroutine
		
endrecursion:
		movq	-16(%rbp), %rax
		
		movq	%rbp, %rsp			#Epilogue: Clear local variables from the stack
		popq	%rbp				#Restore caller's base pointer
		ret	
		
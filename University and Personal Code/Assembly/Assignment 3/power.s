.text
askbase: .asciz "Please give a valid integer for the base:\n"		#String asking for base
askexp: .asciz "Please give a valid integer for the exponent:\n"	#String asking for exponent
formatstr: .asciz "%ld"												#String for scanf
output: .asciz "The total is %d\n"									#String for output

.global main

main:
		pushq	%rbp				#Initializing the base pointer
		movq	%rsp, %rbp
		
		subq	$32, %rsp			#Reserve space for 2 local variables
		
loop1:
		movq	$0, %rax			#No vector registers in use for printf
		movq	$askbase, %rdi		#Loading the adress of askbase
		call	printf				#Calling the printf routine
		
		leaq	-16(%rbp), %rsi		#Loads adress of first local variable to RSI
		movq	$formatstr, %rdi	#Loads first argument of scanf
		movq	$0, %rax			#No vector registers for scanf
		call	scanf				#Calls scanf

		cmpq	$0, -16(%rbp)		#Compare input to 0
		jl		loop1				#If the value of the input is less than 0, ask for input again
		
loop2:		
		movq	$0, %rax			#No vector registers in use for printf
		movq	$askexp, %rdi		#Loading the adress of askexp
		call	printf				#Calling the printf routine
		
		leaq	-32(%rbp), %rsi		#Loads adress of second local variable to RSI
		movq	$formatstr, %rdi	#Loads first argument of scanf
		movq	$0, %rax			#No vector registers for scanf
		call	scanf				#Calls scanf
		
		cmpq	$0, -32(%rbp)		#Compare input to 0
		jl		loop2				#If the value of the imput is less than 0, ask for input again

		movq	-32(%rbp), %rsi		#Loads second argument
		movq	-16(%rbp), %rdi		#Loads first argument
		call 	pow					#Call pow function
		
end:
		mov		$0, %rdi			#Loading the program exit code
		call	exit				#Exiting the program
		
pow:
		pushq	%rbp				#Prologue: pushes old base pointer
		movq	%rsp, %rbp			#Sets new base pointer to current stack pointer
		
		movq	$1, %rax			#initialize total at 1
		
loop3:
		cmpq	$0, %rsi			#Compare rsi (exponent) to 0
		jle		lp3end				#If exponent is equeal to or less than 0, jump to lp3end
		
		mul		%rdi				#Mulptily rax by rdi (base) and plaxe in rax
		dec		%rsi				#Decrement rsi
		
		jmp		loop3				#Jump back to loop3
		
lp3end:	
		movq	%rax, %rsi			#Loads the value of total
		movq	$0, %rax			#No Vector registers in use for printf
		movq	$output, %rdi		#Loading the adress of output string
		call 	printf				#Calls printf
		
		movq	%rbp, %rsp			#Epilogue: Clear local variables from the stack
		popq	%rbp				#Restore caller's base pointer
		ret							#Return from subroutine
		
.text
mystring: .asciz "Tije Eijgenhuijsen, 2705887, your first simple program\n"	#The string we will print later

.global main

main:
		pushq	%rbp
		movq	%rsp, %rbp

		movq	$0, %rax			#No vector registers in use for printf
		movq	$mystring, %rdi		#Loading the adress of mystring
		call	printf				#Calling the printf routine

end:
		mov		$0, %rdi			#Loading the program exit code
		call	exit				#exiting the program
		
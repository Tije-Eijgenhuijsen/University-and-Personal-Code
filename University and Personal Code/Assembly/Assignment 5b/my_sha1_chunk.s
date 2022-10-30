.global sha1_chunk

sha1_chunk:
		pushq	%rbp				#Prologue: pushes old base pointer
		movq	%rsp, %rbp			#Sets new base pointer to current stack pointer
		
		subq	$48, %rsp
		movq	%rsi, -32(%rbp)		#Loads adress of w[0] into -32(%rbp)
		movq	%rdi, -16(%rbp)		#Loads adress of h0 into -16(%rbp)
		
		movq	$16, -48(%rbp)

loop1:
		movq	-48(%rbp), %rdx
		
		call	do_xor
		call	do_xor
		call	do_xor

		inc		-48(%rbp)
		cmpq	$80, -48(%rbp)
		jl		loop1
		
#--------------------------------------------		
		
		
		
		movq	%rbp, %rsp			#Epilogue: Clear local variables from the stack
		popq	%rbp				#Restore caller's base pointer
		ret		
		
		
do_xor:
		pushq	%rbp				#Prologue: pushes old base pointer
		movq	%rsp, %rbp			#Sets new base pointer to current stack pointer

		movq	%rbp, %rsp			#Epilogue: Clear local variables from the stack
		popq	%rbp				#Restore caller's base pointer
		ret	


		
		# 1. %RDI = Adress h0
		# 2. %RSI = Adress w[0]
		# 3. %RDX
		# 4. %RCX
		# 5. %R8
		# 6. %R9
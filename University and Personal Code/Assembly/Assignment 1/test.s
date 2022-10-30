# ************************************************************************
# * Program name : s i e v e *
# * D e s c ri p ti o n : t h i s program p r i n t s a l l the prime numbers below 1000 *
# ************************************************************************
.bss
NUMBERS: .skip 1000 # memory sp ac e f o r the number t a b l e

.text
formatstr: .asciz "%d\n"

.global main
# ************************************************************************
# * S u b r o u ti n e : main *
# * D e s c ri p ti o n : a p p l i c a t i o n e n t r y p oi n t *
# ************************************************************************
main:
	pushq 	%rbp # s t o r e the c a l l e r ’ s b a se p oi n t e r
	movq 	%rsp , %rbp # i n i t i a l i z e the b a se p oi n t e r
# I n i t i a l i z e the number t a b l e :
	movq 	$0 , %rbx # i n i t i a l i z e ’ i ’ t o 0 .
loop1:
	movb 	$1 , NUMBERS(%rbx ) # s e t number t a b l e e n t r y ’ i ’ t o ’ t r u e ’
	incq 	%rbx # inc remen t ’ i ’
	cmpq 	$1000 , %rbx # w hil e ’ i ’ < 1000
	jl		loop1 # go t o s t a r t o f l o o p 1
# The s i e v e al g o ri t hm :
	pushq 	$2 # i n i t i a l i z e ’ number ’ t o 2 on s t a c k
loop2:
	movq 	-8(%rbp ) , %rbx # l o a d ’ number ’ i n t o a r e g i s t e r
	cmpb 	$1 , NUMBERS(%rbx ) # compare NUMBERS[ number ] t o ’ 1 ’
	jne 	lp2end # i f not equal , jump t o end o f l o o p 2
	movq 	$formatstr, %rdi # f i r s t argument : f o rm a t s t r
	movq 	%rbx , %rsi # sec ond argument : the number
	movq 	$0 , %rax # no v e c t o r arguments
	call 	printf # p r i n t the number
	movq 	-8(%rbp ) , %rbx # ’ m ul ti pl e ’ := ’ number ’
	shlq 	$1 , %rbx # m ul ti pl y ’ m ul ti pl e ’ by 2
loop3:
	cmpq 	$1000 , %rbx # compare ’ m ul ti pl e ’ t o 1000
	jge 	lp2end # g o t o end o f l o o p 2 i f g r e a t e r / e q u al
	movb 	$0 , NUMBERS(%rbx ) # s e t number t a b l e e n t r y t o ’ f a l s e ’
	addq 	-8(%rbp ) , %rbx # add an o the r ’ number ’ t o ’ m ul ti pl e ’
	jmp 	loop3 # jump t o the b e gi n ni n g o f l o o p 3
lp2end :
	movq 	-8(%rbp ) , %rbx # l o a d ’ number ’ i n t o a r e g i s t e r
	incq 	%rbx # inc remen t ’ number ’ by one
	movq 	%rbx , -8(%rbp ) # s t o r e ’ number ’ on the s t a c k
	cmpq 	$1000 , %rbx # compare ’ number ’ t o 1000
	jl 		loop2 # i f sm all e r , r e p e a t l o o p 2
end :
	mov 	$0 , %rdi # l o a d program e x i t code
	call 	exit # e x i t the program

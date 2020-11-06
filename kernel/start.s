.section .text
.global start
.type start, @function

start:
	inc %eax
	inc %eax
	call kernel_main
	movl $0xcafecafe, %edi
	inc %eax
	inc %eax
	inc %eax
	inc %eax
	inc %eax
	inc %eax
	inc %eax
	inc %eax
	inc %eax
	hlt 

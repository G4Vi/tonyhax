
#include <regdef.h>

.text
.globl __start

HEADER_ADDR = 0x801FB800
LOAD_ADDR = 0x801FC000

# s0 = A table address
# s1 = file descriptor
# s2 = realstart physical address
# s3 = B table address (on MCINIT only)

__start:
	# Reset the pointer
	li sp, 0x801FFFF0

	# Clear stack
	li t0, LOAD_ADDR
	li t1, 0x80200000 - LOAD_ADDR

clearstack:
	sw zero, 0(t0)
	add t0, 4
	sub t1, 4
	bne t1, zero, clearstack

	# a0 = load address
	# a1 = param1
	# a2 = param2
	li a0, HEADER_ADDR
	li a1, 0
	li a2, 0

	# 0x43 = DoExecute
	li t1, 0x43
	li t0, 0xA0
	jr t0

/*
            Copyright Oliver Kowalke 2009.
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

/****************************************************************************************
 *                                                                                      *
 *  ----------------------------------------------------------------------------------  *
 *  |    0    |    1    |    2    |    3    |    4     |    5    |    6    |    7    |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x0   |   0x4   |   0x8   |   0xc   |   0x10   |   0x14  |   0x18  |   0x1c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  | fc_mxcsr|fc_x87_cw|        R12        |         R13        |        R14        |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |    8    |    9    |   10    |   11    |    12    |    13   |    14   |    15   |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x20  |   0x24  |   0x28  |  0x2c   |   0x30   |   0x34  |   0x38  |   0x3c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        R15        |        RBX        |         RBP        |        RIP        |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |    16   |    17   |         |         |          |         |         |         |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x40  |   0x44  |         |         |          |         |         |         |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        RSP        |                   |                    |                   |  *
 *  ----------------------------------------------------------------------------------  *
 *                                                                                      *
 ****************************************************************************************/

.text
.globl jump_fcontext_1
.type jump_fcontext_1,@function
.align 16
jump_fcontext_1:
    popq  %r8 /* get return address and remove it from stack */

    stmxcsr  (%rdi)     /* save MMX control- and status-word */
    fnstcw   0x4(%rdi)  /* save x87 control-word */

    movq  %r12, 0x8(%rdi)  /* save R12 */
    movq  %r13, 0x10(%rdi)  /* save R13 */
    movq  %r14, 0x18(%rdi)  /* save R14 */
    movq  %r15, 0x20(%rdi)  /* save R15 */
    movq  %rbx, 0x28(%rdi)  /* save RBX */
    movq  %rbp, 0x30(%rdi)  /* save RBP */
    movq  %r8,  0x38(%rdi)  /* save return-address */
    movq  %rsp, 0x40(%rdi)  /* save stack pointrer */

    ldmxcsr  (%rsi)     /* restore MMX control- and status-word */
    fldcw    0x4(%rsi)  /* restore x87 control-word */

    movq  0x8(%rsi), %r12  /* restore R12 */
    movq  0x10(%rsi), %r13  /* restore R13 */
    movq  0x18(%rsi), %r14  /* restore R14 */
    movq  0x20(%rsi), %r15  /* restore R15 */
    movq  0x28(%rsi), %rbx  /* restore RBX */
    movq  0x30(%rsi), %rbp  /* restore RBP */
    movq  0x38(%rsi), %r8  /* restore return-address */
    movq  0x40(%rsi), %rsp  /* restore stack pointer */

    /* return param (data) from jump */
    movq  %rdx, %rax
    /* pass it as the first argument to the fiber function */
    movq  %rax, %rdi

    /* indirect jump to the fiber */
    jmp  *%r8
.size jump_fcontext_1,.-jump_fcontext_1

/* Mark that we don't need executable stack.  */
.section .note.GNU-stack,"",%progbits

/*
            This is modified version of

            Copyright Oliver Kowalke 2009.
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

/**********************************************
 *                                            *
 *  ----------------------------------------  *
 * |       -0x08       |   return (exit)   |  *
 *  ----------------------------------------  *
 * |        0x00       | fc_mxcsr|fc_x87_cw|  *
 *  ----------------------------------------  *
 * |        0x08       |        R12        |  *
 *  ----------------------------------------  *
 * |        0x10       |        R13        |  *
 *  ----------------------------------------  *
 * |        0x18       |        R14        |  *
 *  ----------------------------------------  *
 * |        0x20       |        R15        |  *
 *  ----------------------------------------  *
 * |        0x28       |        RBX        |  *
 *  ----------------------------------------  *
 * |        0x30       |        RBP        |  *
 *  ----------------------------------------  *
 * |        0x38       |        RIP        |  *
 *  ----------------------------------------  *
 * |        0x40       |        RSP        |  *
 *  ----------------------------------------  *
 *                                            *
 **********************************************/

.text
.globl make_fcontext_1
.type make_fcontext_1,@function
.align 16
make_fcontext_1:
    /* first arg of make_fcontext() == bottom of context-stack */
    movq  %rdi, %rax

    /* move rax to the top of the stack */
    addq  %rsi, %rax

    /* shift address in RAX to lower 16 byte boundary */
    andq  $-16, %rax

    /* reserve space for context-data on context-stack */
    /* on context-function entry: (RSP -0x8) % 16 == 0 */
    leaq  -0x50(%rax), %rax

    /* store stack pointer in context */
    movq  %rax, 0x40(%rax)

    /* third arg of make_fcontext() == address of context-function */
    /* stored in RBX */
    movq  %rdx, 0x28(%rax)

    /* save MMX control- and status-word */
    stmxcsr  (%rax)
    /* save x87 control-word */
    fnstcw   0x4(%rax)

    /* compute abs address of label trampoline */
    leaq  trampoline(%rip), %rcx
    /* save address of trampoline as return-address for context-function */
    /* will be entered after calling jump_fcontext() first time */
    movq  %rcx, 0x38(%rax)

    /* compute abs address of label finish */
    leaq  finish(%rip), %rcx
    /* save address of finish as return-address for context-function */
    /* will be entered after context-function returns */
    movq  %rcx, 0x30(%rax)
    movq  %rcx, -0x8(%rax)

    ret /* return pointer to context-data */

trampoline:
    /* store return address on stack */
    /* fix stack alignment */
    push %rbp
    /* jump to context-function */
    jmp *%rbx

finish:
    /* exit code is zero */
    xorq  %rdi, %rdi
    /* exit application */
    call  _exit@PLT
    hlt
.size make_fcontext_1,.-make_fcontext_1

/* Mark that we don't need executable stack. */
.section .note.GNU-stack,"",%progbits

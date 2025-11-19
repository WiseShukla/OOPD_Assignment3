# syscall.s - GAS syntax
.section .text
.global syscall3
.global syscall4

syscall3:
    movq %rdi, %rax
    movq %rsi, %rdi
    movq %rdx, %rsi
    movq %rcx, %rdx
    syscall
    ret

syscall4:
    movq %rdi, %rax     # syscall number
    movq %rsi, %rdi     # arg1
    movq %rdx, %rsi     # arg2
    movq %rcx, %rdx     # arg3
    movq %r8, %r10      # arg4 (note: r10 instead of rcx for syscall)
    syscall
    ret

# Mark stack as non-executable
.section .note.GNU-stack,"",@progbits

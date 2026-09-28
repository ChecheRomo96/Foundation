    .syntax unified
    .cpu cortex-m0plus
    .thumb

    .section .isr_vector,"a",%progbits
    .type FoundationVectorTable, %object
    .global FoundationVectorTable
FoundationVectorTable:
    .word _estack
    .word Reset_Handler
    .word Default_Handler
    .word Default_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler
    .word 0
    .word 0
    .word Default_Handler
    .word Default_Handler
    .size FoundationVectorTable, . - FoundationVectorTable

    .section .text.Default_Handler,"ax",%progbits
    .thumb_func
    .weak Default_Handler
    .type Default_Handler, %function
Default_Handler:
    b Default_Handler
    .size Default_Handler, . - Default_Handler

    .section .note.GNU-stack,"",%progbits

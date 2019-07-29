;    Coffee Kernel Development - Bryan Webb 2019 
;    File:     asm/asmfunc.asm
;    Purpose:  x86 assembly code that is called by the kernel

bits 32

section .text
     global inportb
     global outportb
     
     inportb:
          mov  edx, [esp + 4] ; Pull the arguments off the stack into edx
          in   al, dx         ; Lower byte of eax becomes our return value
                              ; dx is the lower 2 bytes of edx, which is our port
          ret                 ; Return
          
     outportb:
          mov  edx, [esp + 4] ; Pop off the first stack int
          mov  al, [esp + 8]  ; Pop off the second stack int
          out  dx, al
          ret
          
; END .text
          
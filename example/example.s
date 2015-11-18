.fun:
  mov A, 0xff
  mov X, 0xff
  
jmp fun
cmp A, 0xff
jne
jle
jl k
jge
je .ret

.ret
  call 0x00

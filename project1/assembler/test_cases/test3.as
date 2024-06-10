        lw      0       1       one     load reg1 with 1 (symbolic address)    
        lw      0       2       result  load reg1 with 2 (symbolic address)
        nor     0       2       2       nor  reg0 with reg2 (reg2 1's complement)
        add     1       2       2       add reg1 to reg2 (reg2 2's complement)
        sw      0       2       result  store reg2 to result
        halt
one     .fill   1
result  .fill   2

        lw      0       1       two     load leg1 with 2 (symbolic address)
        lw      0       2       three   load reg2 with 3 (symbolic address)
        lw      0       3       result  load reg3 with 0 (symbolic address)
        add     1       2       3       add reg1 and reg2 to reg3 (reg3 = 2 + 3)
        sw      0       3       result  store reg3  to  result     
        halt       
two     .fill   2 
three   .fill   3
result  .fill   0

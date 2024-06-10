        lw      0       1       count   load reg1 with 1 (symbolic address)
        lw      0       2       bound   load reg2 with 6 (symbolic address)
        lw      0       3       result  load reg3 with 0 (symbolic address)
        lw      0       4       one     load reg4 with 1 (symbolic address)
loop    add     1       3       3       reg3 += reg1
        add     1       4       1       reg1 += 1
        beq     1       2       1       if reg1 == reg2, goto store        
        beq     0       0       loop    go back to the beginning of the loop
        sw      0       3       result  store reg4 to result (add 1 ~ bound-1, 1부터 bound-1까지의 합)
        halt              
bound   .fill   6                       
count   .fill   1
result  .fill   0
one     .fill   1

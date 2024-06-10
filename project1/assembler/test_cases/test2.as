        lw      0       1       n1      load reg1 with 2 (symbolic address)
        lw      0       2       n2      load reg2 with 10 (symbolic address)
        lw      0       3       one     load reg3 with 1 (symbolic address)
        lw      0       4       count   load reg4 with 0 (symbolic address)
        lw      0       5       result  load reg5 with 0 (symbolic address)
        lw      0       6       lpAddr  load reg6 with loop address : 6 (symbolic address)
loop    add     1       5       5       reg5 += reg1
        add     3       4       4       reg4 += reg3
        beq     2       4       1       if reg2 == reg4, goto store
        jalr    6       7               go back to the beginning of the loop
        sw      0       5       result  store reg4 to result (multiply n1 with n2 : result = n1 * n2)          
        halt
n1      .fill   2
n2      .fill   10
one     .fill   1
count   .fill   0
result  .fill   0
lpAddr  .fill   loop

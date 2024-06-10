        lw      0       1       five    load reg1 with 5 (symbolic address)
        lw      0       2       six     load reg2 with -1 (numeric address)
        nor     1       five    six       Error - Non-integer register names
five    .fill   5
six     .fill   6

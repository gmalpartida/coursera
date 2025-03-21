// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// zero out R2
@R2
M=0

// check for R0 = 0
@R0
D=M
@END
D;JEQ

// check for R1 = 0
@R1
D=M
@END
D;JEQ

// count = R1
@count
M=D

(LOOP)
    // end when count = 0
    @count
    D=M
    @END
    D;JEQ

    // add R0 to R2
    @R2
    D=M
    @R0
    D=D+M
    @R2
    M=D

    // decrement count
    @count
    M=M-1

    // repeat
    @LOOP
    0;JMP

(END)
@END
    0;JMP
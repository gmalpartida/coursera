// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.


(READ_KEY)


@8192               // 8192 words to be written
D=A
@max_words          // max words = 8192
M=D

@n
M=0                 // n = 0


@KBD    
D=M

@FILL_SCREEN
D;JGT

@CLEAR_SCREEN
0;JMP

(FILL_SCREEN)
@n
D=M

@max_words
D=M-D               // max_words - n

@READ_KEY                // if 0 then goto end
D;JEQ

@SCREEN
D=A
@n
D=D+M

A=D
M=-1

@n
M=M+1

@FILL_SCREEN
0;JMP

(CLEAR_SCREEN)
@n
D=M

@max_words
D=M-D               // max_words - n

@READ_KEY                // if 0 then goto end
D;JEQ

@SCREEN
D=A
@n
D=D+M

A=D
M=0

@n
M=M+1

@CLEAR_SCREEN
0;JMP

(END)
@END
0;JMP
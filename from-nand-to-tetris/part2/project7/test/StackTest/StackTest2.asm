// push constant i
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant i
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq | gt | lt
@SP
M=M-1
A=M
D=M
A=A-1
D=D-M
@SP
M=M-1
@EQUAL
D;JEQ
@SP
A=M
M=0
@END_EQUAL
0;JMP
(EQUAL)
@SP
A=M
M=1
(END_EQUAL)
@SP
M=M+1


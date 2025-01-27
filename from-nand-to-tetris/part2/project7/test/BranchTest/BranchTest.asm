// label TEST_LABEL
(TEST_LABEL)
// push constant 3
@3
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 5
@5
D=A
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
AM=M-1
D=M
A=A-1
M=D+M
// push constant 9
@9
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
AM=M-1
D=M
A=A-1
M=M-D
// goto TEST_LABEL
@TEST_LABEL
0;JMP
// push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 9
@9
D=A
@SP
A=M
M=D
@SP
M=M+1
// and | or
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
A=M
D=D&A
@SP
A=M
M=D
@SP
M=M+1
// if-goto TEST_LABEL
@SP
AM=M-1
D=M
@TEST_LABEL
D;JNE
// pop temp 3
@5
D=A
@3
D=D+A
@R15
M=D
@SP
AM=M-1
D=M
@R15
A=M
M=D
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// push argument 0
@0        //@i
D=A        // #D=i
@ARG        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// push constant 2
@2
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
D=M-D
@SP
M=M-1
@EQUAL3
D;JGE
@SP
A=M
M=-1
@END_EQUAL3
0;JMP
(EQUAL3)
@SP
A=M
M=0
(END_EQUAL3)
@SP
M=M+1
// if-goto N_LT_2
@SP
AM=M-1
D=M
@N_LT_2
D;JNE
// goto N_GE_2
@N_GE_2
0;JMP
// label N_LT_2
(N_LT_2)
// push argument 0
@0        //@i
D=A        // #D=i
@ARG        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// label N_GE_2
(N_GE_2)
// push argument 0
@0        //@i
D=A        // #D=i
@ARG        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// push constant 2
@2
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
// call Main.fibonacci 1
(Main.fibonacci)
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@ARG
D=M
@SP
A=M
M=D
@THIS
D=M
@SP
A=M
M=D
@THAT
D=M
@SP
A=M
M=D
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
// push argument 0
@0        //@i
D=A        // #D=i
@ARG        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// push constant 1
@1
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
// call Main.fibonacci 1
(Main.fibonacci)
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@ARG
D=M
@SP
A=M
M=D
@THIS
D=M
@SP
A=M
M=D
@THAT
D=M
@SP
A=M
M=D
@SP
D=M
@5
D=D-A
@1
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
// add
@SP
AM=M-1
D=M
A=A-1
M=D+M

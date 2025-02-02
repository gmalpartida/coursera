// function Main.fibonacci 0
(Main.fibonacci)
@0
D=A
(loop_0)
@loop_end_0
D;JEQ
@SP
A=M
M=0
@SP
M=M+1
D=D-1
@loop
0;JMP
(loop_end_0)
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
@EQUAL1
D;JGE
@SP
A=M
M=-1
@END_EQUAL1
0;JMP
(EQUAL1)
@SP
A=M
M=0
(END_EQUAL1)
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
// return
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
@Main.fibonacci$ret.0
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
@Main.fibonacci
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@1
0;JMP
(Main.fibonacci$ret.0)
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
@Main.fibonacci$ret.1
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
@Main.fibonacci
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@1
0;JMP
(Main.fibonacci$ret.1)
// add
@SP
AM=M-1
D=M
A=A-1
M=D+M
// return

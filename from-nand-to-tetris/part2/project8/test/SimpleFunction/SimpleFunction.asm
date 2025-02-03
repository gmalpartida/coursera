// function SimpleFunction.test 2
(SimpleFunction.test)
@2
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
// push local 0
@0        //@i
D=A        // #D=i
@LCL        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// push local 1
@1        //@i
D=A        // #D=i
@LCL        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// add
@SP
AM=M-1
D=M
A=A-1
M=D+M
// not
@SP
M=M-1
A=M
A=M
D=!A
@SP
A=M
M=D
@SP
M=M+1
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
// add
@SP
AM=M-1
D=M
A=A-1
M=D+M
// push argument 1
@1        //@i
D=A        // #D=i
@ARG        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// sub
@SP
AM=M-1
D=M
A=A-1
M=M-D
// return
@LCL
D=M
@endFrame
M=D
D=M
@5
D=D-A
retAddr
M=D
@SP
AM=M-1
D=M
@ARG
M=D
D=A
D=D+1
@SP
M=D
@endFrame
D=A
D=D-1
@THAT
M=D
D=D-1
@THIS
M=D
D=D-1
@ARG
M=D
D=D-1
@LCL
M=D
@retAddr
0;JMP

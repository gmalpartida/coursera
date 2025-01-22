// push constant i
@3030
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop pointer 0/1
@SP
M=M-1
A=M
D=M
@THIS
M=D
// push constant i
@3040
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop pointer 0/1
@SP
M=M-1
A=M
D=M
@THAT
M=D
// push constant i
@32
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop segment i
@THIS
D=M
@2
A=D+A
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// push constant i
@46
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop segment i
@THAT
D=M
@6
A=D+A
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
// push pointer 0/1
@THIS
A=M
D=A
@SP
A=M
M=D
@SP
M=M+1
// push pointer 0/1
@THAT
A=M
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
// push segment i
@2        //@i
D=A        // #D=i
@THIS        // @segment
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
// push segment i
@6        //@i
D=A        // #D=i
@THAT        // @segment
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

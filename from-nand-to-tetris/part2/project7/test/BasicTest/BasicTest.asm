// push constant i
@10
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop segment i
@local
D=M
@0
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
@21
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant i
@22
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop segment i
@argument
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
// pop segment i
@argument
D=M
@1
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
@36
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop segment i
@this
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
// push constant i
@42
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant i
@45
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop segment i
@that
D=M
@5
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
// pop segment i
@that
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
@510
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp i
@5
D=A
@6
D=D+A
@R15
M=D
@SP
AM=M-1
D=M
@R15
A=M
M=D
// push segment i
@0        //@i
D=A        // #D=i
@local        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// push segment i
@5        //@i
D=A        // #D=i
@that        // @segment
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
M=M+D
// push segment i
@1        //@i
D=A        // #D=i
@argument        // @segment
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
@this        // @segment
A=D+M      // A=LCL+i jump to that memory
D=M        // D=LCL[i]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// push segment i
@6        //@i
D=A        // #D=i
@this        // @segment
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
M=M+D
// sub
@SP
AM=M-1
D=M
A=A-1
M=M-D
// push temp i
@6
D=A
@5
A=D+A
D=M
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
M=M+D

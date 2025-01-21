// push constant i
@111
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant i
@333
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant i
@888
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop static i
@SP
AM=M-1
D=M
@StaticTest.8
M=D
// pop static i
@SP
AM=M-1
D=M
@StaticTest.3
M=D
// pop static i
@SP
AM=M-1
D=M
@StaticTest.1
M=D
// push static i
@StaticTest.3        //@file.i
D=M        // D=RAM[var_name]
@SP
A=M        // jump to location of SP
M=D        // RAM[*SP] = D
@SP
M=M+1      // SP++
// push static i
@StaticTest.1        //@file.i
D=M        // D=RAM[var_name]
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
// push static i
@StaticTest.8        //@file.i
D=M        // D=RAM[var_name]
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

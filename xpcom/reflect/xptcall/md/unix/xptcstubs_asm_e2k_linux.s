.section   .text
.globl     SharedStub
.type      SharedStub,@function
.hidden    SharedStub

SharedStub:
/*
 * r0 - that(a0)
 * r1-r7 - register arguments(a1-a7)
 * stack[0] - stub number
 * stack[1] - stack arguments
 */

{
    disp %ctpr1, PrepareAndDispatch
    setwd wsz = 0xa, nfx = 0x1, dbl = 0x0
    setbn rsz = 0x3, rbs = 0x6, rcur = 0x0
    getsp,0     -0x50, %r9
}
{
    addd,0      %r9, 0x50, %r8
    addd,1,sm   %r0, 0x0,  %b[0]
}
{
    ldd,0,sm    [%r8 + 0x40], %b[1]
    ldd,2,sm    [%r8 + 0x48], %b[2]
    addd,1,sm   %r9, 0x0,  %b[3]
}
{
    std,2,sm    %r0, [%r9 + 0x0]
    std,5,sm    %r1, [%r9 + 0x8]
}
{
    std,2,sm    %r2, [%r9 + 0x10]
    std,5,sm    %r3, [%r9 + 0x18]
}
{
    std,2,sm    %r4, [%r9 + 0x20]
    std,5,sm    %r5, [%r9 + 0x28]
}
{
    std,2,sm    %r6, [%r9 + 0x30]
    std,5,sm    %r7, [%r9 + 0x38]
    call        %ctpr1, wbs=0x6
}
{
    return      %ctpr3
    nop 5
    addd,3,sm   %b[0], 0x0, %r0
}
{
    ct          %ctpr3
}
.size      SharedStub,.-SharedStub

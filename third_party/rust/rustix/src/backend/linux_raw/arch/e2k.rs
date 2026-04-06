//! e2k Linux system calls.

use crate::backend::reg::{
    ArgReg, FromAsm, RetReg, SyscallNumber, ToAsm, A0, A1, A2, A3, A4, A5, R0,
};
use core::arch::asm;

#[inline]
pub(in crate::backend) unsafe fn syscall0_readonly(nr: SyscallNumber<'_>) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[1]") _,
        out("b[2]") _,
        out("b[3]") _,
        out("b[4]") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags, readonly)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall1(nr: SyscallNumber<'_>, a0: ArgReg<'_, A0>) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[2]") _,
        out("b[3]") _,
        out("b[4]") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall1_readonly(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[2]") _,
        out("b[3]") _,
        out("b[4]") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags, readonly)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall1_noreturn(nr: SyscallNumber<'_>, a0: ArgReg<'_, A0>) -> ! {
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        in("b[0]") nr.to_asm(),
        in("b[1]") a0.to_asm(),
        options(nostack, noreturn)
    )
}

#[inline]
pub(in crate::backend) unsafe fn syscall2(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[3]") _,
        out("b[4]") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall2_readonly(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[3]") _,
        out("b[4]") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags, readonly)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall3(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[4]") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall3_readonly(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[4]") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags, readonly)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall4(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
    a3: ArgReg<'_, A3>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        inout("b[4]") a3.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall4_readonly(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
    a3: ArgReg<'_, A3>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        inout("b[4]") a3.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[5]") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags, readonly)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall5(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
    a3: ArgReg<'_, A3>,
    a4: ArgReg<'_, A4>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        inout("b[4]") a3.to_asm() => _,
        inout("b[5]") a4.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall5_readonly(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
    a3: ArgReg<'_, A3>,
    a4: ArgReg<'_, A4>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        inout("b[4]") a3.to_asm() => _,
        inout("b[5]") a4.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[6]") _,
        out("b[7]") _,
        options(nostack, preserves_flags, readonly)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall6(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
    a3: ArgReg<'_, A3>,
    a4: ArgReg<'_, A4>,
    a5: ArgReg<'_, A5>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        inout("b[4]") a3.to_asm() => _,
        inout("b[5]") a4.to_asm() => _,
        inout("b[6]") a5.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[7]") _,
        options(nostack, preserves_flags)
    );
    FromAsm::from_asm(r0)
}

#[inline]
pub(in crate::backend) unsafe fn syscall6_readonly(
    nr: SyscallNumber<'_>,
    a0: ArgReg<'_, A0>,
    a1: ArgReg<'_, A1>,
    a2: ArgReg<'_, A2>,
    a3: ArgReg<'_, A3>,
    a4: ArgReg<'_, A4>,
    a5: ArgReg<'_, A5>,
) -> RetReg<R0> {
    let r0;
    asm!(
        "sdisp %ctpr1, 0x3",
        "call %ctpr1, wbs = %#",
        inout("b[0]") nr.to_asm() => r0,
        inout("b[1]") a0.to_asm() => _,
        inout("b[2]") a1.to_asm() => _,
        inout("b[3]") a2.to_asm() => _,
        inout("b[4]") a3.to_asm() => _,
        inout("b[5]") a4.to_asm() => _,
        inout("b[6]") a5.to_asm() => _,
        out("ctpr1") _,
        out("ctpr2") _,
        out("ctpr3") _,
        out("b[7]") _,
        options(nostack, preserves_flags, readonly)
    );
    FromAsm::from_asm(r0)
}

from ctypes import c_uint64 as u64
from ctypes import c_double as lf


def bytes2lf_divs(fn_rng64) -> float:
    return (u64(fn_rng64()).value & 0x000F_FFFF_FFFF_FFFF) / 4503599627370496.0


def bytes2lf_mult(fn_rng64) -> float:
    return (u64(fn_rng64()).value & 0x000F_FFFF_FFFF_FFFF) * float.fromhex("0x1p-52")


def bytes2lf_fill(fn_rng64) -> float:
    u = 0x3FF0_0000_0000_0000 | (u64(fn_rng64()).value & 0x000F_FFFF_FFFF_FFFF)
    x = lf.from_buffer(u64(u)).value
    return x - 1


def bytes2lf_expl(fn_rng64) -> float:
    leading_zeros = 0
    for _ in range(17):  # 64*17 = 17
        if u := u64(fn_rng64()):
            leading_zeros += 64 - u.value.bit_length()
            break
        leading_zeros += 64
    else:
        return 0.0

    exponent = 1023 - 1 - leading_zeros
    if exponent <= 0:
        return 0.0

    mantissa = u64(fn_rng64()).value & 0x000F_FFFF_FFFF_FFFF
    u = (u64(exponent).value << 52) | mantissa
    return lf.from_buffer(u64(u)).value

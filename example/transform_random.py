import numpy as np
from math import sqrt, log, sin, cos, pi
from typing import Callable
from ctypes import c_uint64 as u64
from ctypes import c_double as lf
from numpy.typing import NDArray as nparr


def bytes2lf_divs(fn_rng64: Callable[..., int]) -> float:
    return (u64(fn_rng64()).value & 0x000F_FFFF_FFFF_FFFF) / 4503599627370496.0


def bytes2lf_mult(fn_rng64: Callable[..., int]) -> float:
    return (u64(fn_rng64()).value & 0x000F_FFFF_FFFF_FFFF) * float.fromhex("0x1p-52")


def bytes2lf_fill(fn_rng64: Callable[..., int]) -> float:
    u = 0x3FF0_0000_0000_0000 | (u64(fn_rng64()).value & 0x000F_FFFF_FFFF_FFFF)
    x = lf.from_buffer(u64(u)).value
    return x - 1


def bytes2lf_expl(fn_rng64: Callable[..., int]) -> float:
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


# box-muller transformation for two flaot
def u01_to_normal(a: float, b: float) -> tuple[float, float]:
    x1 = sqrt(-2 * log(a)) * cos(2 * pi * b)
    x2 = sqrt(-2 * log(a)) * sin(2 * pi * b)
    return (x1, x2)


# box-muller transformation for numpy array
def np_arry_u01_to_normal(
    arr: nparr[np.float64],
) -> nparr[np.float64]:
    arr = arr[:-1] if len(arr) % 2 else arr
    a1 = arr[: len(arr) // 2]
    a2 = arr[len(arr) // 2 :]

    x1 = np.sqrt(-2 * np.log(a1)) * np.cos(2 * np.pi * a2)
    x2 = np.sqrt(-2 * np.log(a1)) * np.sin(2 * np.pi * a2)
    return np.concatenate((x1, x2), axis=0)


# transform normal to arbitrary normal
def np_array_arbitrary_normal(
    arr: nparr[np.float64], mean: int | float = 0, stddev: int | float = 1
) -> nparr[np.float64]:
    if stddev == 0:
        raise ValueError("stddev cannot be 0")
    return mean + stddev * arr

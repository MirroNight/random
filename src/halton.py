from sympy.ntheory import isprime


def halton(n: int, b: int) -> float:
    # get n-th number in Halton series of base b
    if not isprime(b):
        raise ValueError("base must be prime")

    f, r = 1.0, 0.0
    while n > 0:
        f /= b
        r += f * (n % b)
        n //= b
    return r


def halton_series(n: int, b: int):
    # generates Halton series of base b up to n-th number
    # primarily for generating a large Halton series
    if not isprime(b):
        raise ValueError("base must be prime")

    i = 0
    arr = [0]
    while (i := i + 1) < n:
        for d in range(len(arr)):
            if arr[d] < b - 1:
                arr[d] += 1
                break
            else:
                arr[d] = 0
        else:
            arr.append(1)
        f, r = 1, 0.0
        for d in arr:
            f /= b
            r += f * d
        yield r

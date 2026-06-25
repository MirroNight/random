import transform_random
from struct import unpack


def get_rand_fn(f):
    def u64_fn():
        u64 = f.read(8)
        while True:
            if len(u64) < 8:
                raise EOFError
            return unpack(">Q", u64)[0]

    return u64_fn


def main():
    with open("/dev/urandom", "rb") as f:
        rng64 = get_rand_fn(f)
        for _ in range(10):
            try:
                # lf = bytes2lf.bytes2lf_divs(rng64)
                # lf = bytes2lf.bytes2lf_mult(rng64)
                # lf = bytes2lf.bytes2lf_fill(rng64)
                lf = transform_random.bytes2lf_expl(rng64)
                print(f"{lf}")
            except EOFError:
                break
    return 0


if __name__ == "__main__":
    main()

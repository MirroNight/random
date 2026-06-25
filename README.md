# Randoms

This is a repository for a few different pseudorandom number generators and related utilities.

The directories contains:

- `src`: source code for RNGs and utilities.
- `example`: examples of how to use them.
- `lib`: a pcg random linkable object compiled from [PCG random](https://www.pcg-random.org/index.html).
- `bin`: the compiled example binaries.

## Sources

PRNGs:

- `chacha20.h`      : chacha20 PRNG
- `lfsr.h`          : lfsr PRNG
- `minstd.h`        : minstd PRNG
- `mt19937.h`       : mt19937 PRNG
- `pcg_variants.h`  : pcgrandom PRNG
- `xorshift.h`      : xorshift PRNG

Utilities:

- `endian_test.h`   : test endianess of the machine
- `halton.py`       : halton series generator
- `bytes2lf.h`      : bytes to double conversion

To use each source, simply copy the file into your project and include it in your code, no library compilation and linking required (except for `PCG`).

## Examples

The example directory contains examples of how to use each PRNG / utility.

### Build examples

```sh
make
# or
make debug
```

### Run example

Replace `<PRNG>` with PRNG name.

```sh

./bin/<PRNG>_example | head -c 256 | xxd

./bin/<PRNG>_example | ./bin/bytes2lf_example | head -n 10

python example/halton_example.py
```


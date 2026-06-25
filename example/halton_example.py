import halton


def main():
    cnt = 16
    base = 2
    for f in halton.halton_series(cnt, base):
        print(f"{f:<.10}")
    return 0


if __name__ == "__main__":
    main()

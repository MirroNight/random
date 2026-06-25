import transform_random
import numpy as np
import matplotlib.pyplot as plt


def main():
    a, b = np.random.random(2)
    c, d = transform_random.u01_to_normal(a, b)
    print(f"{a}\t{b}\n{c}\t{d}\n")

    plt.figure()
    plt.grid()

    arrU0 = np.random.random(1000)
    plt.hist(arrU0, bins=100, color="black", alpha=0.5, label="Uniform")
    plt.ylabel("count")
    plt.locator_params(axis="y", integer=True)
    plt.legend()
    plt.show()

    arrN1 = transform_random.np_arry_u01_to_normal(arrU0)
    arrN2 = transform_random.np_array_arbitrary_normal(arrN1, mean=4, stddev=2)

    plt.hist(arrN1, bins=100, color="blue", alpha=0.3, label="Normal")
    plt.hist(arrN2, bins=100, color="green", alpha=0.3, label="Normal+")
    plt.ylabel("count")
    plt.legend()
    plt.show()
    plt.clf()
    return 0


if __name__ == "__main__":
    main()

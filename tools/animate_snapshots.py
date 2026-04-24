import sys
import glob
import time
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def load_field_csv(path):
    df = pd.read_csv(path)
    n = int(max(df["x"].max(), df["y"].max())) + 1

    def grid(name):
        return df.pivot(index="x", columns="y", values=name).values

    Psi_re = grid("Psi_re")
    Psi_im = grid("Psi_im")

    return Psi_re**2 + Psi_im**2  # density

def main():
    files = sorted(glob.glob("output/step_*_fields.csv"))

    plt.ion()
    fig, ax = plt.subplots()

    for f in files:
        data = load_field_csv(f)
        ax.clear()
        ax.imshow(data, origin="lower")
        ax.set_title(f)
        plt.pause(0.05)

    plt.ioff()
    plt.show()

if __name__ == "__main__":
    main()
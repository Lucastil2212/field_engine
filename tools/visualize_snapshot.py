#!/usr/bin/env python3
"""
Unified Field Engine
File: tools/visualize_snapshot.py

Visualizes one CSV snapshot produced by the C field engine.

Usage:
    python3 tools/visualize_snapshot.py output/step_100_fields.csv
"""

import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def load_field_csv(path):
    df = pd.read_csv(path)

    n = int(max(df["x"].max(), df["y"].max())) + 1

    def grid(name):
        return df.pivot(index="x", columns="y", values=name).values

    A = grid("A")
    Theta = grid("Theta")
    Phi = grid("Phi")
    Pi = grid("Pi")
    Psi_re = grid("Psi_re")
    Psi_im = grid("Psi_im")

    density = Psi_re**2 + Psi_im**2

    return {
        "n": n,
        "A": A,
        "Theta": Theta,
        "Phi": Phi,
        "Pi": Pi,
        "density": density,
    }


def plot_field(fields, title):
    fig, axes = plt.subplots(2, 3, figsize=(14, 9))
    fig.suptitle(title, fontsize=16)

    panels = [
        ("A: amplitude", fields["A"]),
        ("Theta: phase", np.angle(np.exp(1j * fields["Theta"]))),
        ("Phi: self-similarity", fields["Phi"]),
        ("Pi: prime field", fields["Pi"]),
        ("|Psi|^2: density", fields["density"]),
    ]

    for ax, (name, data) in zip(axes.flat, panels):
        im = ax.imshow(data, origin="lower")
        ax.set_title(name)
        ax.set_xticks([])
        ax.set_yticks([])
        fig.colorbar(im, ax=ax, fraction=0.046, pad=0.04)

    axes.flat[-1].axis("off")

    plt.tight_layout()
    plt.show()


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 tools/visualize_snapshot.py output/step_100_fields.csv")
        return 1

    path = sys.argv[1]
    fields = load_field_csv(path)
    plot_field(fields, path)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
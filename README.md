# Unified Field Engine

A real-time computational simulator implementing an ontology-native unified field model.

---

## Overview

The **Unified Field Engine** is a C-based simulation system that models a structured substrate field:

[
\Psi = A \cdot e^{i\Theta} \cdot \Phi \cdot \Pi
]

This field is not treated as a traditional physical field (like EM or gravity), but as a **generative substrate** from which observable sectors emerge.

The system evolves this substrate over time and provides:

* Real-time visualization
* Structural invariant tracking
* Experimental sandbox for unified field hypotheses

---

## Core Idea

Instead of modeling separate physical forces, this engine models a **single structured field** composed of four fundamental components:

| Component     | Meaning                                           |
| ------------- | ------------------------------------------------- |
| **A**         | Amplitude (energy / intensity)                    |
| **Θ (Theta)** | Phase (cyclic structure / time generator)         |
| **Φ (Phi)**   | Self-similarity (recursive / attractor structure) |
| **Π (Pi)**    | Discrete structure (prime-based arithmetic field) |

From this decomposition, all observable behavior is hypothesized to emerge.

---

## Architecture

### Data Model

Each simulation cell stores:

```txt
A[x,y]        amplitude
Theta[x,y]    phase
Phi[x,y]      recursive structure
nu[p][x,y]    prime valuation channels
Pi[x,y]       multiplicative prime field
Psi[x,y]      reconstructed complex field
```

Where:

[
\Pi = \exp\left(\sum_p \nu_p \log p \right)
]

---

### Simulation Pipeline

Each frame:

1. Reconstruct field:
   [
   \Psi = A \cdot e^{i\Theta} \cdot \Phi \cdot \Pi
   ]

2. Compute structural kernels:

   * Phase curvature
   * Φ attractor dynamics
   * Prime valuation smoothing
   * Counter-rotation proxy

3. Update fields via finite-difference evolution

4. Compute invariants:

   * Energy
   * Phase structure
   * Self-similarity deviation
   * Prime structure
   * Counter-rotation
   * Entropy

---

## Features

### Real-Time Renderer (raylib)

* Live evolving field visualization
* Multiple modes:

  * `1` Density |Ψ|²
  * `2` Phase Θ
  * `3` Φ deviation
* Controls:

  * `SPACE` pause
  * `R` reset
* Displays live invariants

---

### Offline Analysis

* CSV snapshot export
* Python visualization tools
* Animation support

---

## Build Instructions

### Requirements

* GCC (C11)
* raylib
* Linux (X11 libraries)

### Build

```bash
make clean
make
```

---

## Run

### Real-time renderer

```bash
./field_live
```

### Offline simulation

```bash
./field_engine --n 128 --steps 1000 --output-every 100
```

---

## Visualization (Python)

Install dependencies:

```bash
pip install numpy pandas matplotlib
```

View a snapshot:

```bash
python3 tools/visualize_snapshot.py output/step_100_fields.csv
```

---

## What You're Observing

This engine is not just visualizing data — it is exploring hypotheses.

### Key Phenomena

#### Phase Structure (Θ)

* Reveals vortices and winding numbers
* Encodes time-like behavior

#### Counter-Rotation

* Opposing rotations cancel globally but preserve local structure
* Candidate mechanism for quantum-like behavior

#### Φ Attractor

* System tends toward golden ratio equilibrium
* Models recursive stability

#### Prime Structure (Π)

* Introduces discrete arithmetic influence
* Potential link to quantization

#### Entropy vs Structure

* Tests whether order persists under entropy growth

---

## Current Limitations

* Simplified kernels (toy model)
* No explicit physical unit calibration
* Counter-rotation is approximate
* No spectral decomposition yet
* CPU-only implementation

---

## Future Directions

### 1. Physics Derivation

* Extract electromagnetic fields from ∇Θ
* Recover Schrödinger-like behavior
* Test geometric coupling → gravity analog

---

### 2. Mathematical Refinement

* Replace heuristic kernels with variational formulation
* Define full action functional
* Formalize symmetry group invariants

---

### 3. Visualization Upgrades

* Vector field rendering
* Flow lines / streamlines
* Phase wheel overlays
* Multi-channel composite rendering

---

### 4. Performance

* OpenMP parallelization
* GPU acceleration (CUDA / OpenCL)
* Adaptive timestep integration

---

### 5. Simulation Extensions

* 3D field support
* Non-periodic boundary conditions
* Multi-scale coupling
* External perturbations

---

### 6. Experimental Program

* Measure stability of counter-rotation structures
* Detect persistent invariants under perturbation
* Study emergent pattern formation
* Analyze entropy vs coherence tradeoffs

---

## Philosophical Position

This project treats physics as **emergent from structured fields**, not fundamental forces.

It is:

* Not a proof
* Not a final theory

It is a **computational laboratory** for exploring whether:

> Structured phase, recursion, and discreteness can generate the phenomena we call reality.

---

## Project Status

* Core engine: complete
* Real-time rendering: functional
* Visualization: working
* Experimental platform: active

---

## Contributing

Future contributions may include:

* Kernel improvements
* Visualization enhancements
* GPU backend
* Mathematical formalization
* Experimental validation tools

---

## Final Note

You are not just running a simulation.

You are exploring a **hypothesis about the structure of reality** — one frame at a time.

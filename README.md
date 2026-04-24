# Unified Field Engine

A real-time computational simulator for an ontology-native unified field model.

---

## Overview

The **Unified Field Engine** is a low-level (C-based) simulation system that models a **single structured substrate field** from which multiple physical-like behaviors emerge.

Rather than modeling separate forces (electromagnetism, gravity, quantum fields), this system treats reality as the evolution of a **unified structured field**:

$$
\Psi = A \cdot e^{i\Theta} \cdot \Phi \cdot \Pi
$$

This project serves as:

* A **computational ontology**
* A **physics sandbox**
* A **field dynamics laboratory**

---

## Core Concept

The field is decomposed into four fundamental components:

| Component | Description                                       |
| --------- | ------------------------------------------------- |
| (A)       | Amplitude (energy / intensity)                    |
| (\Theta)  | Phase (cyclic structure, time generator)          |
| (\Phi)    | Self-similarity (recursive attractor structure)   |
| (\Pi)     | Discrete structure (prime-based arithmetic field) |

The full field is reconstructed as:

$$
\Psi(x,y) = A(x,y),e^{i\Theta(x,y)},\Phi(x,y),\Pi(x,y)
$$

---

## Discrete Arithmetic Field

The discrete component is built using prime valuation:

$$
\Pi = \exp\left(\sum_{p} \nu_p \log p \right)
$$

Where:

* (p) = prime numbers
* (\nu_p) = local valuation fields

This introduces **arithmetic structure directly into the field**.

---

## Architecture

### Data Representation

Each grid cell contains:

```txt
A[x,y]        amplitude
Theta[x,y]    phase
Phi[x,y]      recursive structure
nu[p][x,y]    prime valuation channels
Pi[x,y]       multiplicative field
Psi[x,y]      complex reconstructed field
```

---

### Simulation Loop

Each iteration performs:

1. Field reconstruction:
   $$
   \Psi = A \cdot e^{i\Theta} \cdot \Phi \cdot \Pi
   $$

2. Kernel evaluation:

   * Phase curvature
   * Phi attractor force
   * Prime valuation smoothing
   * Counter-rotation proxy

3. Field updates (finite differences)

4. Invariant computation

---

## Structural Invariants

The engine tracks key structural quantities:

### Energy

$$
E[\Psi]
$$

### Phase Structure

$$
S_\Theta = \sum |\nabla \Theta|^2
$$

### Self-Similarity

$$
S_\Phi = \sum (\Phi - \varphi)^2
$$

Where:

$$
\varphi = \frac{1 + \sqrt{5}}{2}
$$

---

### Prime Structure

$$
S_p = \sum_p \sum |\nabla \nu_p|^2
$$

---

### Counter-Rotation

$$
S_{\pm}
$$

Measures local rotational cancellation and shear structure.

---

### Entropy

$$
H[\Psi] = -\sum \rho_\Psi \log \rho_\Psi
$$

Where:

$$
\rho_\Psi = \frac{|\Psi|^2}{\sum |\Psi|^2}
$$

---

## Real-Time Renderer

Built using **raylib**, the engine provides live visualization.

### Modes

| Key | Mode             |      |      |
| --- | ---------------- | ---- | ---- |
| `1` | Density (        | \Psi | ^2 ) |
| `2` | Phase ( \Theta ) |      |      |
| `3` | Phi deviation    |      |      |

---

### Controls

```txt
SPACE   pause/resume
R       reset simulation
1/2/3   switch visualization mode
```

---

## Build Instructions

### Requirements

* GCC (C11)
* raylib
* Linux (X11 libraries)

---

### Build

```bash
make clean
make
```

---

## Run

### Real-time simulation

```bash
./field_live
```

---

### Offline simulation

```bash
./field_engine --n 128 --steps 1000 --output-every 100
```

---

## Python Visualization

Install:

```bash
pip install numpy pandas matplotlib
```

Run:

```bash
python3 tools/visualize_snapshot.py output/step_100_fields.csv
```

---

## What You Are Observing

This system is not just visual—it is **experimental**.

### Phase (Θ)

* Reveals vortices
* Encodes cyclic structure
* Acts as a time-like variable

---

### Counter-Rotation

* Opposing rotations cancel globally
* Local structure remains
* Possible analog to quantum behavior

---

### Φ Field

* Attractor toward golden ratio
* Encodes recursive structure
* Stability mechanism

---

### Prime Field (Π)

* Introduces discrete structure
* May relate to quantization
* Links arithmetic and dynamics

---

### Entropy vs Structure

Tests whether:

> Structure can persist under entropy increase

---

## Current Limitations

* Simplified kernels (not derived from action principle)
* Approximate counter-rotation measure
* No explicit unit calibration
* CPU-only
* 2D only

---

## Future Directions

### 1. Physics Extraction

* Derive EM fields from ( \nabla \Theta )
* Recover Schrödinger-like equations
* Explore curvature → gravity analog

---

### 2. Mathematical Formalization

* Define full action functional
* Replace heuristic kernels with variational dynamics
* Formal symmetry group analysis

---

### 3. Visualization

* Vector field rendering
* Flow lines
* Phase wheel overlays
* Multi-channel compositing

---

### 4. Performance

* OpenMP parallelization
* GPU acceleration (CUDA / OpenCL)
* Adaptive timestep solvers

---

### 5. Simulation Expansion

* 3D fields
* Boundary conditions (non-periodic)
* Multi-scale coupling
* External perturbations

---

### 6. Experimental Program

* Stability of counter-rotating structures
* Emergence of persistent patterns
* Entropy vs coherence tradeoffs
* Phase-driven structure formation

---

## Philosophical Position

This project explores the idea that:

> Reality is not built from forces, but from structured field states.

It treats:

* Phase as fundamental
* Recursion as stabilizing
* Arithmetic as structural
* Geometry as emergent

---

## Status

| Component             | Status   |
| --------------------- | -------- |
| Core engine           | Complete |
| Real-time rendering   | Working  |
| Visualization         | Working  |
| Experimental platform | Active   |

---

## Final Note

This is not a finished theory.

It is a **tool for discovering one**.

Every frame is a hypothesis.

Every pattern is a clue.

---

## License

This project is licensed under the Apache License 2.0.

See the LICENSE file for details.


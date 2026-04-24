#ifndef UNIFIED_FIELD_ENGINE_FIELD_H
#define UNIFIED_FIELD_ENGINE_FIELD_H

/*
 * Unified Field Engine
 * File: include/field.h
 *
 * Core substrate declaration.
 *
 * The engine evolves the ontology-native field:
 *
 *     Psi = A * exp(i * Theta) * Phi * Pi
 *
 * with:
 *   A      : amplitude / intensity
 *   Theta  : phase / cyclic ordering
 *   Phi    : self-similar recursive structure
 *   nu     : prime valuation channels
 *   Pi     : arithmetic/discrete multiplicative field
 *   Psi    : reconstructed complex substrate
 */

#include <stddef.h>

#define FIELD_DEFAULT_PRIME_COUNT 3

/*
 * First canonical prime channels.
 *
 * These are the initial arithmetic basis used by the engine.
 * Later versions may load arbitrary prime channels dynamically.
 */
#define FIELD_PRIME_0 2.0
#define FIELD_PRIME_1 3.0
#define FIELD_PRIME_2 5.0

/*
 * Golden ratio fixed point:
 *
 *     phi^2 = phi + 1
 */
#define FIELD_GOLDEN_RATIO 1.61803398874989484820

typedef struct {
  int n;
  double dt;

  /*
   * Prime valuation basis.
   *
   * primes[p] stores the p-th prime channel value.
   * nu[p, x, y] stores the local valuation amplitude for that channel.
   */
  int prime_count;
  double *primes;
  double *nu;

  /*
   * Substrate factors.
   */
  double *A;
  double *Theta;
  double *Phi;
  double *Pi;

  /*
   * Reconstructed complex substrate:
   *
   *     Psi_re + i Psi_im
   */
  double *Psi_re;
  double *Psi_im;
} Field;

/*
 * Flat indexing helpers for an n x n periodic lattice.
 */
#define FIELD_IDX(field_ptr, x, y) (((x) * (field_ptr)->n) + (y))

#define FIELD_NU_IDX(field_ptr, p, x, y)                                       \
  (((p) * (field_ptr)->n * (field_ptr)->n) + FIELD_IDX((field_ptr), (x), (y)))

/*
 * Periodic wrapping helper.
 *
 * Safe for x in {-1, 0, ..., n}.
 * For larger offsets, use modulo explicitly.
 */
static inline int field_wrap(int x, int n) {
  if (x < 0)
    return n - 1;
  if (x >= n)
    return 0;
  return x;
}

/*
 * Lifecycle.
 */
int field_init(Field *field, int n, double dt);
void field_free(Field *field);

/*
 * Reconstruct derived fields:
 *
 *     Pi  = exp(sum_p nu_p log(prime_p))
 *     Psi = A * exp(iTheta) * Phi * Pi
 */
void field_rebuild(Field *field);

/*
 * Initial conditions.
 */
void field_seed_vacuum(Field *field);
void field_seed_counter_rotating_vortices(Field *field, double strength);

/*
 * Utility.
 */
size_t field_cell_count(const Field *field);
size_t field_prime_cell_count(const Field *field);

#endif
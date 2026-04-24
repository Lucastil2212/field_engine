/*
 * Unified Field Engine
 * File: src/field.c
 *
 * Implements the substrate:
 *
 *     Psi = A * exp(i Theta) * Phi * Pi
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "field.h"

/*
 * Internal allocation helper
 */
static double *alloc_array(size_t count) {
  double *ptr = (double *)calloc(count, sizeof(double));
  return ptr;
}

size_t field_cell_count(const Field *field) {
  return (size_t)field->n * (size_t)field->n;
}

size_t field_prime_cell_count(const Field *field) {
  return (size_t)field->prime_count * field_cell_count(field);
}

/*
 * Initialize field
 */
int field_init(Field *field, int n, double dt) {
  if (!field || n <= 0)
    return -1;

  field->n = n;
  field->dt = dt;

  size_t cells = (size_t)n * (size_t)n;

  field->prime_count = FIELD_DEFAULT_PRIME_COUNT;

  /*
   * Allocate prime basis
   */
  field->primes = alloc_array(field->prime_count);
  if (!field->primes)
    return -1;

  field->primes[0] = FIELD_PRIME_0;
  field->primes[1] = FIELD_PRIME_1;
  field->primes[2] = FIELD_PRIME_2;

  /*
   * Allocate fields
   */
  field->A = alloc_array(cells);
  field->Theta = alloc_array(cells);
  field->Phi = alloc_array(cells);
  field->Pi = alloc_array(cells);
  field->Psi_re = alloc_array(cells);
  field->Psi_im = alloc_array(cells);

  field->nu = alloc_array(field_prime_cell_count(field));

  if (!field->A || !field->Theta || !field->Phi || !field->Pi ||
      !field->Psi_re || !field->Psi_im || !field->nu) {
    return -1;
  }

  /*
   * Initialize vacuum
   *
   * Psi_vac = phi
   */
  field_seed_vacuum(field);

  return 0;
}

/*
 * Free memory
 */
void field_free(Field *field) {
  if (!field)
    return;

  free(field->A);
  free(field->Theta);
  free(field->Phi);
  free(field->Pi);
  free(field->Psi_re);
  free(field->Psi_im);
  free(field->nu);
  free(field->primes);

  memset(field, 0, sizeof(Field));
}

/*
 * Vacuum state
 *
 * A = 1
 * Theta = 0
 * Phi = golden ratio
 * nu = 0
 */
void field_seed_vacuum(Field *field) {
  size_t cells = field_cell_count(field);

  for (size_t i = 0; i < cells; i++) {
    field->A[i] = 1.0;
    field->Theta[i] = 0.0;
    field->Phi[i] = FIELD_GOLDEN_RATIO;
    field->Pi[i] = 1.0;

    field->Psi_re[i] = FIELD_GOLDEN_RATIO;
    field->Psi_im[i] = 0.0;
  }

  memset(field->nu, 0, field_prime_cell_count(field) * sizeof(double));
}

/*
 * Build Pi and Psi from factors
 */
void field_rebuild(Field *field) {
  int n = field->n;

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {

      int idx = FIELD_IDX(field, x, y);

      /*
       * Build log(Pi)
       */
      double log_pi = 0.0;

      for (int p = 0; p < field->prime_count; p++) {
        int nuidx = FIELD_NU_IDX(field, p, x, y);
        log_pi += field->nu[nuidx] * log(field->primes[p]);
      }

      double Pi = exp(log_pi);
      field->Pi[idx] = Pi;

      /*
       * Build Psi
       */
      double A = field->A[idx];
      double Theta = field->Theta[idx];
      double Phi = field->Phi[idx];

      double amp = A * Phi * Pi;

      field->Psi_re[idx] = amp * cos(Theta);
      field->Psi_im[idx] = amp * sin(Theta);
    }
  }
}

/*
 * Counter-rotating vortex seed
 *
 * This encodes:
 *   +m and -m winding simultaneously
 */
void field_seed_counter_rotating_vortices(Field *field, double strength) {
  int n = field->n;

  double center = (double)(n - 1) / 2.0;

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {

      int idx = FIELD_IDX(field, x, y);

      double dx = (double)x - center;
      double dy = (double)y - center;

      /*
       * Two offset vortex centers
       */
      double dx1 = dx;
      double dy1 = dy - n * 0.15;

      double dx2 = dx;
      double dy2 = dy + n * 0.15;

      double theta1 = atan2(dy1, dx1);
      double theta2 = -atan2(dy2, dx2);

      field->Theta[idx] = strength * (theta1 + theta2);

      /*
       * Slight amplitude bump at center
       */
      double r2 = dx * dx + dy * dy;
      field->A[idx] = 1.0 + 0.2 * exp(-r2 / (0.1 * n * n));

      /*
       * Keep Phi near golden attractor
       */
      field->Phi[idx] = FIELD_GOLDEN_RATIO;

      double wave = sin(0.15 * x) * cos(0.15 * y);

      field->Phi[idx] = FIELD_GOLDEN_RATIO + 0.05 * wave;

      for (int p = 0; p < field->prime_count; p++) {
        int nuidx = FIELD_NU_IDX(field, p, x, y);
        field->nu[nuidx] = 0.05 * sin(0.05 * field->primes[p] * x) *
                           cos(0.05 * field->primes[p] * y);
      }
    }
  }
}
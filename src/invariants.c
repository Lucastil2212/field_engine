/*
 * Unified Field Engine
 * File: src/invariants.c
 *
 * Computes structural invariants of the substrate.
 */

#include <math.h>
#include <stdlib.h>

#include "invariants.h"
#include "kernels.h"

/*
 * Aggregate all invariants
 */
FieldInvariants invariants_compute(const Field *field) {
  FieldInvariants inv;

  inv.energy = invariants_energy(field);
  inv.phase_structure = invariants_phase_structure(field);
  inv.self_similarity = invariants_self_similarity(field);
  inv.prime_structure = invariants_prime_structure(field);
  inv.counter_rotation = invariants_counter_rotation(field);
  inv.entropy = invariants_entropy(field);

  return inv;
}

/*
 * Energy proxy:
 *
 * Combination of amplitude, gradients, phase gradients, Phi gradients, and nu
 * gradients
 */
double invariants_energy(const Field *field) {
  int n = field->n;

  double energy = 0.0;

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {

      int idx = FIELD_IDX(field, x, y);

      double A = field->A[idx];

      double gradA2 = kernels_grad2_at(field, field->A, x, y);
      double gradTheta2 = kernels_grad2_at(field, field->Theta, x, y);
      double gradPhi2 = kernels_grad2_at(field, field->Phi, x, y);

      energy += A * A + gradA2 + A * A * gradTheta2 + gradPhi2;
    }
  }

  /* add prime valuation gradient contribution */
  for (int p = 0; p < field->prime_count; p++) {
    const double *nu_p = &field->nu[(size_t)p * field_cell_count(field)];

    for (int x = 0; x < n; x++) {
      for (int y = 0; y < n; y++) {
        energy += kernels_grad2_at(field, nu_p, x, y);
      }
    }
  }

  return energy;
}

/*
 * Phase structure:
 *
 * ∫ |∇Theta|^2
 */
double invariants_phase_structure(const Field *field) {
  int n = field->n;

  double sum = 0.0;

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      sum += kernels_grad2_at(field, field->Theta, x, y);
    }
  }

  return sum;
}

/*
 * Self-similarity structure:
 *
 * ∫ (Phi - phi)^2
 */
double invariants_self_similarity(const Field *field) {
  int n = field->n;

  double sum = 0.0;

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      int idx = FIELD_IDX(field, x, y);

      double d = field->Phi[idx] - FIELD_GOLDEN_RATIO;
      sum += d * d;
    }
  }

  return sum;
}

/*
 * Prime structure:
 *
 * ∑_p ∫ |∇nu_p|^2
 */
double invariants_prime_structure(const Field *field) {
  int n = field->n;

  double sum = 0.0;

  for (int p = 0; p < field->prime_count; p++) {
    const double *nu_p = &field->nu[(size_t)p * field_cell_count(field)];

    for (int x = 0; x < n; x++) {
      for (int y = 0; y < n; y++) {
        sum += kernels_grad2_at(field, nu_p, x, y);
      }
    }
  }

  return sum;
}

/*
 * Counter-rotation proxy:
 *
 * ∫ |K_pm|
 */
double invariants_counter_rotation(const Field *field) {
  int n = field->n;

  double sum = 0.0;

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      double val = kernels_counter_rotation_proxy_at(field, x, y);
      sum += fabs(val);
    }
  }

  return sum;
}

/*
 * Entropy:
 *
 * H = - ∑ rho log rho
 * where rho = |Psi|^2 / ∑ |Psi|^2
 */
double invariants_entropy(const Field *field) {
  int n = field->n;
  size_t cells = field_cell_count(field);

  double total = 0.0;

  /* compute total density */
  for (size_t i = 0; i < cells; i++) {
    double re = field->Psi_re[i];
    double im = field->Psi_im[i];
    total += re * re + im * im;
  }

  if (total <= 0.0)
    return 0.0;

  double entropy = 0.0;

  for (size_t i = 0; i < cells; i++) {
    double re = field->Psi_re[i];
    double im = field->Psi_im[i];

    double rho = (re * re + im * im) / total;

    if (rho > 1e-12) {
      entropy -= rho * log(rho);
    }
  }

  return entropy;
}
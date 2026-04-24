/*
 * Unified Field Engine
 * File: src/kernels.c
 *
 * Implements finite-difference evolution of the substrate:
 *
 *     Psi = A * exp(i Theta) * Phi * Pi
 */

#include <math.h>
#include <stdlib.h>

#include "kernels.h"

static double clamp_double(double x, double min_value, double max_value) {
  if (x < min_value)
    return min_value;
  if (x > max_value)
    return max_value;
  return x;
}

KernelParams kernels_default_params(void) {
  KernelParams params;

  params.diffusion_A = 0.025;
  params.diffusion_theta = 0.050;
  params.diffusion_phi = 0.020;
  params.diffusion_nu = 0.015;

  params.lambda_theta = 0.20;
  params.lambda_phi = 0.12;
  params.lambda_p = 0.04;
  params.lambda_pm = 0.02;

  params.damping_A = 0.001;
  params.damping_theta = 0.000;
  params.damping_phi = 0.002;
  params.damping_nu = 0.003;

  params.min_A = 1.0e-9;
  params.min_Phi = 1.0e-9;
  params.max_abs_nu = 4.0;

  return params;
}

double kernels_laplacian_at(const Field *field, const double *f, int x, int y) {
  int n = field->n;

  int xm = field_wrap(x - 1, n);
  int xp = field_wrap(x + 1, n);
  int ym = field_wrap(y - 1, n);
  int yp = field_wrap(y + 1, n);

  int c = FIELD_IDX(field, x, y);
  int il = FIELD_IDX(field, xm, y);
  int ir = FIELD_IDX(field, xp, y);
  int id = FIELD_IDX(field, x, ym);
  int iu = FIELD_IDX(field, x, yp);

  return f[il] + f[ir] + f[id] + f[iu] - 4.0 * f[c];
}

double kernels_grad_x_at(const Field *field, const double *f, int x, int y) {
  int n = field->n;

  int xm = field_wrap(x - 1, n);
  int xp = field_wrap(x + 1, n);

  return 0.5 * (f[FIELD_IDX(field, xp, y)] - f[FIELD_IDX(field, xm, y)]);
}

double kernels_grad_y_at(const Field *field, const double *f, int x, int y) {
  int n = field->n;

  int ym = field_wrap(y - 1, n);
  int yp = field_wrap(y + 1, n);

  return 0.5 * (f[FIELD_IDX(field, x, yp)] - f[FIELD_IDX(field, x, ym)]);
}

double kernels_grad2_at(const Field *field, const double *f, int x, int y) {
  double gx = kernels_grad_x_at(field, f, x, y);
  double gy = kernels_grad_y_at(field, f, x, y);

  return gx * gx + gy * gy;
}

double kernels_phase_curvature_at(const Field *field, int x, int y) {
  return kernels_laplacian_at(field, field->Theta, x, y);
}

double kernels_phi_attractor_at(const Field *field, int x, int y) {
  int idx = FIELD_IDX(field, x, y);
  double phi = field->Phi[idx];

  /*
   * Golden-ratio attractor:
   *
   *     Phi^2 - Phi - 1 = 0
   */
  return phi * phi - phi - 1.0;
}

double kernels_prime_smoothing_at(const Field *field, int p, int x, int y) {
  const double *nu_p = &field->nu[(size_t)p * field_cell_count(field)];
  return kernels_laplacian_at(field, nu_p, x, y);
}

double kernels_counter_rotation_proxy_at(const Field *field, int x, int y) {
  /*
   * v0 counter-rotation proxy:
   *
   * We measure local twist/shear by crossing first derivatives of phase.
   * This is not yet full spectral positive/negative winding extraction.
   */
  double tx = kernels_grad_x_at(field, field->Theta, x, y);
  double ty = kernels_grad_y_at(field, field->Theta, x, y);

  double ax = kernels_grad_x_at(field, field->A, x, y);
  double ay = kernels_grad_y_at(field, field->A, x, y);

  return tx * ay - ty * ax;
}

void kernels_step(Field *field) {
  KernelParams params = kernels_default_params();
  kernels_step_with_params(field, &params);
}

void kernels_step_with_params(Field *field, const KernelParams *params) {
  int n = field->n;
  double dt = field->dt;
  size_t cells = field_cell_count(field);
  size_t prime_cells = field_prime_cell_count(field);

  double *next_A = (double *)malloc(cells * sizeof(double));
  double *next_Theta = (double *)malloc(cells * sizeof(double));
  double *next_Phi = (double *)malloc(cells * sizeof(double));
  double *next_nu = (double *)malloc(prime_cells * sizeof(double));

  if (!next_A || !next_Theta || !next_Phi || !next_nu) {
    free(next_A);
    free(next_Theta);
    free(next_Phi);
    free(next_nu);
    return;
  }

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      int idx = FIELD_IDX(field, x, y);

      double A = field->A[idx];
      double Theta = field->Theta[idx];
      double Phi = field->Phi[idx];

      double lap_A = kernels_laplacian_at(field, field->A, x, y);
      double lap_Theta = kernels_laplacian_at(field, field->Theta, x, y);
      double lap_Phi = kernels_laplacian_at(field, field->Phi, x, y);

      double K_theta = kernels_phase_curvature_at(field, x, y);
      double K_phi = kernels_phi_attractor_at(field, x, y);
      double K_pm = kernels_counter_rotation_proxy_at(field, x, y);

      /*
       * Amplitude evolution:
       *
       * Smooth amplitude, weakly damp deviations from vacuum amplitude,
       * and allow counter-rotational structure to feed local intensity.
       */
      next_A[idx] = A + dt * (params->diffusion_A * lap_A -
                              params->damping_A * (A - 1.0) +
                              params->lambda_pm * fabs(K_pm));

      /*
       * Phase evolution:
       *
       * Phase curvature diffuses while the explicit phase kernel
       * strengthens coherent phase transport.
       */
      next_Theta[idx] = Theta + dt * (params->diffusion_theta * lap_Theta +
                                      params->lambda_theta * K_theta -
                                      params->damping_theta * Theta);

      /*
       * Self-similarity evolution:
       *
       * Phi is pulled toward the golden-ratio fixed point:
       *
       *     Phi^2 - Phi - 1 = 0
       */
      next_Phi[idx] =
          Phi +
          dt * (params->diffusion_phi * lap_Phi - params->lambda_phi * K_phi -
                params->damping_phi * (Phi - FIELD_GOLDEN_RATIO));

      next_A[idx] = clamp_double(next_A[idx], params->min_A, 1.0e9);
      next_Phi[idx] = clamp_double(next_Phi[idx], params->min_Phi, 1.0e9);
    }
  }

  for (int p = 0; p < field->prime_count; p++) {
    const double *nu_p = &field->nu[(size_t)p * cells];

    for (int x = 0; x < n; x++) {
      for (int y = 0; y < n; y++) {
        int idx = FIELD_IDX(field, x, y);
        int nuidx = FIELD_NU_IDX(field, p, x, y);

        double nu = nu_p[idx];
        double lap_nu = kernels_laplacian_at(field, nu_p, x, y);

        /*
         * Prime valuation evolution:
         *
         * Smooth local arithmetic valuation while damping it toward
         * the neutral valuation vacuum.
         */
        next_nu[nuidx] =
            nu + dt * (params->diffusion_nu * lap_nu +
                       params->lambda_p * lap_nu - params->damping_nu * nu);

        next_nu[nuidx] = clamp_double(next_nu[nuidx], -params->max_abs_nu,
                                      params->max_abs_nu);
      }
    }
  }

  for (size_t i = 0; i < cells; i++) {
    field->A[i] = next_A[i];
    field->Theta[i] = next_Theta[i];
    field->Phi[i] = next_Phi[i];
  }

  for (size_t i = 0; i < prime_cells; i++) {
    field->nu[i] = next_nu[i];
  }

  free(next_A);
  free(next_Theta);
  free(next_Phi);
  free(next_nu);
}
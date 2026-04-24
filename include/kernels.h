#ifndef UNIFIED_FIELD_ENGINE_KERNELS_H
#define UNIFIED_FIELD_ENGINE_KERNELS_H

/*
 * Unified Field Engine
 * File: include/kernels.h
 *
 * Numerical kernels for the ontology-native substrate.
 *
 * These are the update operators for:
 *
 *   A      amplitude / intensity
 *   Theta  phase / cyclic ordering
 *   Phi    self-similar recursive structure
 *   nu     prime valuation channels
 *
 * The engine uses a 2D periodic lattice and finite-difference stencils.
 */

#include "field.h"

/*
 * Runtime coupling constants.
 *
 * These are deliberately explicit so we can tune the ontology sectors
 * without hiding assumptions inside the code.
 */
typedef struct {
  double diffusion_A;
  double diffusion_theta;
  double diffusion_phi;
  double diffusion_nu;

  double lambda_theta;
  double lambda_phi;
  double lambda_p;
  double lambda_pm;

  double damping_A;
  double damping_theta;
  double damping_phi;
  double damping_nu;

  double min_A;
  double min_Phi;
  double max_abs_nu;
} KernelParams;

/*
 * Default stable toy-model parameters.
 */
KernelParams kernels_default_params(void);

/*
 * Main evolution step using default parameters.
 */
void kernels_step(Field *field);

/*
 * Main evolution step using explicit parameters.
 */
void kernels_step_with_params(Field *field, const KernelParams *params);

/*
 * Finite-difference helpers.
 *
 * These use periodic boundary conditions.
 */
double kernels_laplacian_at(const Field *field, const double *f, int x, int y);
double kernels_grad_x_at(const Field *field, const double *f, int x, int y);
double kernels_grad_y_at(const Field *field, const double *f, int x, int y);
double kernels_grad2_at(const Field *field, const double *f, int x, int y);

/*
 * Individual structural kernels.
 */
double kernels_phase_curvature_at(const Field *field, int x, int y);
double kernels_phi_attractor_at(const Field *field, int x, int y);
double kernels_prime_smoothing_at(const Field *field, int p, int x, int y);

/*
 * Counter-rotation proxy.
 *
 * In the first engine version, this is a local measure of phase-gradient
 * shear/twist. Later we can replace it with a true spectral decomposition
 * into positive and negative winding modes.
 */
double kernels_counter_rotation_proxy_at(const Field *field, int x, int y);

#endif
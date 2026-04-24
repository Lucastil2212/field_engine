#ifndef UNIFIED_FIELD_ENGINE_INVARIANTS_H
#define UNIFIED_FIELD_ENGINE_INVARIANTS_H

/*
 * Unified Field Engine
 * File: include/invariants.h
 *
 * Structural observables / invariants of the substrate:
 *
 *   E        total structured energy proxy
 *   S_theta  phase organization
 *   S_phi    self-similarity deviation from golden-ratio attractor
 *   S_p      prime valuation structure
 *   S_pm     counter-rotation proxy
 *   H        normalized entropy of |Psi|^2
 */

#include "field.h"

typedef struct {
  double energy;
  double phase_structure;
  double self_similarity;
  double prime_structure;
  double counter_rotation;
  double entropy;
} FieldInvariants;

/*
 * Compute all invariants.
 */
FieldInvariants invariants_compute(const Field *field);

/*
 * Individual invariant families.
 */
double invariants_energy(const Field *field);
double invariants_phase_structure(const Field *field);
double invariants_self_similarity(const Field *field);
double invariants_prime_structure(const Field *field);
double invariants_counter_rotation(const Field *field);
double invariants_entropy(const Field *field);

#endif
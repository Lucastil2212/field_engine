#ifndef UNIFIED_FIELD_ENGINE_IO_H
#define UNIFIED_FIELD_ENGINE_IO_H

/*
 * Unified Field Engine
 * File: include/io.h
 *
 * Input / Output layer.
 *
 * Responsibilities:
 *  - Create output directories
 *  - Write simulation snapshots
 *  - Persist invariants + field state
 *
 * v0 format:
 *  - Human-readable CSV (portable, easy to inspect)
 *  - One file per snapshot
 */

#include "field.h"
#include "invariants.h"

/*
 * Create output directory if it does not exist.
 *
 * Returns:
 *   0 on success
 *  -1 on failure
 */
int io_make_output_dir(const char *path);

/*
 * Write a full simulation snapshot.
 *
 * Output files:
 *
 *   <dir>/step_<N>_fields.csv
 *   <dir>/step_<N>_invariants.txt
 *
 * Fields CSV columns:
 *   x, y, A, Theta, Phi, Pi, Psi_re, Psi_im
 *
 * Returns:
 *   0 on success
 *  -1 on failure
 */
int io_write_snapshot(const char *output_dir, int step, const Field *field,
                      const FieldInvariants *inv);

/*
 * Low-level helpers (exposed for flexibility)
 */

int io_write_fields_csv(const char *filepath, const Field *field);

int io_write_invariants_txt(const char *filepath, int step,
                            const FieldInvariants *inv);

#endif
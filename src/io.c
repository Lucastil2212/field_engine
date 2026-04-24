/*
 * Unified Field Engine
 * File: src/io.c
 *
 * Handles filesystem + snapshot serialization.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "io.h"

/*
 * Create directory if it doesn't exist
 */
int io_make_output_dir(const char *path) {
#ifdef _WIN32
  int result = _mkdir(path);
#else
  int result = mkdir(path, 0755);
#endif

  if (result == 0)
    return 0;

  if (errno == EEXIST)
    return 0;

  return -1;
}

/*
 * Build file path helper
 */
static void build_path(char *buffer, size_t size, const char *dir,
                       const char *name) {
  snprintf(buffer, size, "%s/%s", dir, name);
}

/*
 * Write full snapshot
 */
int io_write_snapshot(const char *output_dir, int step, const Field *field,
                      const FieldInvariants *inv) {
  char path_fields[256];
  char path_inv[256];
  char filename[128];

  snprintf(filename, sizeof(filename), "step_%d_fields.csv", step);
  build_path(path_fields, sizeof(path_fields), output_dir, filename);

  snprintf(filename, sizeof(filename), "step_%d_invariants.txt", step);
  build_path(path_inv, sizeof(path_inv), output_dir, filename);

  if (io_write_fields_csv(path_fields, field) != 0)
    return -1;
  if (io_write_invariants_txt(path_inv, step, inv) != 0)
    return -1;

  return 0;
}

/*
 * Write field grid to CSV
 */
int io_write_fields_csv(const char *filepath, const Field *field) {
  FILE *f = fopen(filepath, "w");
  if (!f)
    return -1;

  int n = field->n;

  fprintf(f, "x,y,A,Theta,Phi,Pi,Psi_re,Psi_im\n");

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      int idx = FIELD_IDX(field, x, y);

      fprintf(f, "%d,%d,%.12e,%.12e,%.12e,%.12e,%.12e,%.12e\n", x, y,
              field->A[idx], field->Theta[idx], field->Phi[idx], field->Pi[idx],
              field->Psi_re[idx], field->Psi_im[idx]);
    }
  }

  fclose(f);
  return 0;
}

/*
 * Write invariants
 */
int io_write_invariants_txt(const char *filepath, int step,
                            const FieldInvariants *inv) {
  FILE *f = fopen(filepath, "w");
  if (!f)
    return -1;

  fprintf(f, "step: %d\n", step);
  fprintf(f, "E: %.12e\n", inv->energy);
  fprintf(f, "S_theta: %.12e\n", inv->phase_structure);
  fprintf(f, "S_phi: %.12e\n", inv->self_similarity);
  fprintf(f, "S_p: %.12e\n", inv->prime_structure);
  fprintf(f, "S_pm: %.12e\n", inv->counter_rotation);
  fprintf(f, "H: %.12e\n", inv->entropy);

  fclose(f);
  return 0;
}
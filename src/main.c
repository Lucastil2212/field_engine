/*
 * Unified Field Engine
 * File: src/main.c
 *
 * Entry point for the ontology-native unified field simulator.
 *
 * Core substrate:
 *
 *     Psi = A * exp(i * Theta) * Phi * Pi
 *
 * where:
 *   A      = amplitude / intensity
 *   Theta  = phase / cyclic ordering
 *   Phi    = self-similar recursive structure
 *   Pi     = prime/discrete arithmetic structure
 *
 * This engine treats observable sectors as projections of the substrate,
 * not as independent primitive fields.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "field.h"
#include "invariants.h"
#include "io.h"
#include "kernels.h"

#define DEFAULT_GRID_SIZE 128
#define DEFAULT_STEPS 1000
#define DEFAULT_DT 0.01
#define DEFAULT_OUTPUT_EVERY 100

typedef struct {
  int grid_size;
  int steps;
  int output_every;
  double dt;
  const char *output_dir;
} EngineConfig;

static void print_usage(const char *program_name) {
  printf("Unified Field Engine\n");
  printf("\n");
  printf("Usage:\n");
  printf("  %s [options]\n", program_name);
  printf("\n");
  printf("Options:\n");
  printf("  --n <int>              Grid size. Default: %d\n",
         DEFAULT_GRID_SIZE);
  printf("  --steps <int>          Number of simulation steps. Default: %d\n",
         DEFAULT_STEPS);
  printf("  --dt <float>           Time step. Default: %.4f\n", DEFAULT_DT);
  printf("  --output-every <int>   Snapshot interval. Default: %d\n",
         DEFAULT_OUTPUT_EVERY);
  printf("  --output-dir <path>    Output directory. Default: output\n");
  printf("  --help                 Show this message.\n");
}

static EngineConfig default_config(void) {
  EngineConfig cfg;
  cfg.grid_size = DEFAULT_GRID_SIZE;
  cfg.steps = DEFAULT_STEPS;
  cfg.dt = DEFAULT_DT;
  cfg.output_every = DEFAULT_OUTPUT_EVERY;
  cfg.output_dir = "output";
  return cfg;
}

static int parse_args(int argc, char **argv, EngineConfig *cfg) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--n") == 0) {
      if (i + 1 >= argc)
        return -1;
      cfg->grid_size = atoi(argv[++i]);
    } else if (strcmp(argv[i], "--steps") == 0) {
      if (i + 1 >= argc)
        return -1;
      cfg->steps = atoi(argv[++i]);
    } else if (strcmp(argv[i], "--dt") == 0) {
      if (i + 1 >= argc)
        return -1;
      cfg->dt = atof(argv[++i]);
    } else if (strcmp(argv[i], "--output-every") == 0) {
      if (i + 1 >= argc)
        return -1;
      cfg->output_every = atoi(argv[++i]);
    } else if (strcmp(argv[i], "--output-dir") == 0) {
      if (i + 1 >= argc)
        return -1;
      cfg->output_dir = argv[++i];
    } else if (strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      exit(0);
    } else {
      return -1;
    }
  }

  if (cfg->grid_size <= 0 || cfg->steps <= 0 || cfg->dt <= 0.0 ||
      cfg->output_every <= 0) {
    return -1;
  }

  return 0;
}

static void print_config(const EngineConfig *cfg) {
  printf("Unified Field Engine boot sequence\n");
  printf("----------------------------------\n");
  printf("grid_size    : %d x %d\n", cfg->grid_size, cfg->grid_size);
  printf("steps        : %d\n", cfg->steps);
  printf("dt           : %.8f\n", cfg->dt);
  printf("output_every : %d\n", cfg->output_every);
  printf("output_dir   : %s\n", cfg->output_dir);
  printf("\n");
}

static void print_invariants(int step, const FieldInvariants *inv) {
  printf("step=%d "
         "E=%.12e "
         "S_theta=%.12e "
         "S_phi=%.12e "
         "S_p=%.12e "
         "S_pm=%.12e "
         "H=%.12e\n",
         step, inv->energy, inv->phase_structure, inv->self_similarity,
         inv->prime_structure, inv->counter_rotation, inv->entropy);
}

int main(int argc, char **argv) {
  EngineConfig cfg = default_config();

  if (parse_args(argc, argv, &cfg) != 0) {
    fprintf(stderr, "Invalid arguments.\n\n");
    print_usage(argv[0]);
    return 1;
  }

  print_config(&cfg);

  if (io_make_output_dir(cfg.output_dir) != 0) {
    fprintf(stderr, "Failed to create output directory: %s\n", cfg.output_dir);
    return 1;
  }

  Field field;

  if (field_init(&field, cfg.grid_size, cfg.dt) != 0) {
    fprintf(stderr, "Failed to initialize field.\n");
    return 1;
  }

  /*
   * First canonical experiment:
   * counter-rotating vortex pair.
   *
   * This directly targets the ontology insight:
   * coarse angular bias may cancel while relational structure remains.
   */
  field_seed_counter_rotating_vortices(&field, 2.0);

  field_rebuild(&field);

  FieldInvariants inv = invariants_compute(&field);
  print_invariants(0, &inv);

  if (io_write_snapshot(cfg.output_dir, 0, &field, &inv) != 0) {
    fprintf(stderr, "Warning: failed to write initial snapshot.\n");
  }

  for (int step = 1; step <= cfg.steps; step++) {
    kernels_step(&field);
    field_rebuild(&field);

    if (step % cfg.output_every == 0 || step == cfg.steps) {
      inv = invariants_compute(&field);
      print_invariants(step, &inv);

      if (io_write_snapshot(cfg.output_dir, step, &field, &inv) != 0) {
        fprintf(stderr, "Warning: failed to write snapshot at step %d.\n",
                step);
      }
    }
  }

  field_free(&field);

  printf("\nSimulation complete.\n");
  return 0;
}
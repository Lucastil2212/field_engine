#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "field.h"
#include "invariants.h"
#include "kernels.h"

#define DEFAULT_N 256
#define WINDOW_SCALE 3

static unsigned char clamp_byte(double x) {
  if (x < 0.0)
    return 0;
  if (x > 255.0)
    return 255;
  return (unsigned char)x;
}

static Color color_density(const Field *field, int idx) {
  double re = field->Psi_re[idx];
  double im = field->Psi_im[idx];
  double d = re * re + im * im;

  double v = log(1.0 + d) * 60.0;

  return (Color){clamp_byte(v), clamp_byte(v * 0.65),
                 clamp_byte(255.0 - v * 0.35), 255};
}

static Color color_phase(const Field *field, int idx) {
  double theta = field->Theta[idx];
  double r = 0.5 + 0.5 * cos(theta);
  double g = 0.5 + 0.5 * cos(theta + 2.09439510239);
  double b = 0.5 + 0.5 * cos(theta + 4.18879020479);

  return (Color){clamp_byte(255.0 * r), clamp_byte(255.0 * g),
                 clamp_byte(255.0 * b), 255};
}

static Color color_phi(const Field *field, int idx) {
  double d = fabs(field->Phi[idx] - FIELD_GOLDEN_RATIO);
  double v = d * 900.0;

  return (Color){clamp_byte(v), clamp_byte(255.0 - v),
                 clamp_byte(80.0 + v * 0.25), 255};
}

static void update_pixels(Field *field, Color *pixels, int mode) {
  int n = field->n;
  size_t cells = field_cell_count(field);

  double min_v = 1.0e300;
  double max_v = -1.0e300;

  double *values = malloc(cells * sizeof(double));
  if (!values)
    return;

  for (size_t i = 0; i < cells; i++) {
    if (mode == 0) {
      double re = field->Psi_re[i];
      double im = field->Psi_im[i];
      values[i] = log(1.0 + re * re + im * im);
    } else if (mode == 1) {
      values[i] = field->Theta[i];
    } else {
      values[i] = fabs(field->Phi[i] - FIELD_GOLDEN_RATIO);
    }

    if (values[i] < min_v)
      min_v = values[i];
    if (values[i] > max_v)
      max_v = values[i];
  }

  double range = max_v - min_v;
  if (range < 1.0e-12)
    range = 1.0;

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      int idx = FIELD_IDX(field, x, y);

      if (mode == 1) {
        pixels[idx] = color_phase(field, idx);
      } else {
        double t = (values[idx] - min_v) / range;

        pixels[idx] =
            (Color){clamp_byte(255.0 * t), clamp_byte(255.0 * sqrt(t)),
                    clamp_byte(255.0 * (1.0 - t)), 255};
      }
    }
  }

  free(values);
}

int main(void) {
  int n = DEFAULT_N;

  Field field;
  if (field_init(&field, n, 0.01) != 0) {
    fprintf(stderr, "Failed to initialize field.\n");
    return 1;
  }

  field_seed_counter_rotating_vortices(&field, 2.0);
  field_rebuild(&field);

  Color *pixels = malloc((size_t)n * (size_t)n * sizeof(Color));
  if (!pixels) {
    fprintf(stderr, "Failed to allocate pixels.\n");
    field_free(&field);
    return 1;
  }

  InitWindow(n * WINDOW_SCALE, n * WINDOW_SCALE, "Unified Field Engine - Live");
  SetTargetFPS(60);

  Image image = {.data = pixels,
                 .width = n,
                 .height = n,
                 .mipmaps = 1,
                 .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

  Texture2D texture = LoadTextureFromImage(image);

  int mode = 0;
  int paused = 0;
  int step = 0;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_SPACE))
      paused = !paused;
    if (IsKeyPressed(KEY_ONE))
      mode = 0;
    if (IsKeyPressed(KEY_TWO))
      mode = 1;
    if (IsKeyPressed(KEY_THREE))
      mode = 2;
    if (IsKeyPressed(KEY_R)) {
      field_seed_counter_rotating_vortices(&field, 2.0);
      field_rebuild(&field);
      step = 0;
    }

    if (!paused) {
      for (int i = 0; i < 4; i++) {
        kernels_step(&field);
        field_rebuild(&field);
        step++;
      }
    }

    update_pixels(&field, pixels, mode);
    UpdateTexture(texture, pixels);

    FieldInvariants inv = invariants_compute(&field);

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextureEx(texture, (Vector2){0, 0}, 0.0f, (float)WINDOW_SCALE, WHITE);

    DrawRectangle(10, 10, 520, 118, Fade(BLACK, 0.72f));

    DrawText("Unified Field Engine - Live", 20, 20, 20, RAYWHITE);
    DrawText("1 density |Psi|^2   2 phase Theta   3 Phi deviation   SPACE "
             "pause   R reset",
             20, 46, 14, LIGHTGRAY);

    char line[256];
    snprintf(line, sizeof(line),
             "step=%d  E=%.3e  S_theta=%.3e  S_phi=%.3e  S_pm=%.3e  H=%.3e",
             step, inv.energy, inv.phase_structure, inv.self_similarity,
             inv.counter_rotation, inv.entropy);
    DrawText(line, 20, 72, 14, GREEN);

    const char *mode_name = mode == 0 ? "density" : mode == 1 ? "phase" : "phi";
    snprintf(line, sizeof(line), "mode=%s", mode_name);
    DrawText(line, 20, 96, 14, SKYBLUE);

    EndDrawing();
  }

  UnloadTexture(texture);
  CloseWindow();

  free(pixels);
  field_free(&field);

  return 0;
}
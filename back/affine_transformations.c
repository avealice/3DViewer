#include "affine_transformations.h"

void rotate_on_x_axis(matrix_t *matrix, double angle) {
  angle = angle * M_PI / 180;
  for (int i = 1; i < matrix->rows; i++) {
    double temp_y = matrix->matrix[i][1];
    double temp_z = matrix->matrix[i][2];
    matrix->matrix[i][1] = cos(angle) * temp_y + sin(angle) * temp_z;
    matrix->matrix[i][2] = sin(angle) * (-temp_y) + cos(angle) * temp_z;
  }
}

void rotate_on_y_axis(matrix_t *matrix, double angle) {
  angle = angle * M_PI / 180;
  for (int i = 1; i < matrix->rows; i++) {
    double temp_x = matrix->matrix[i][0];
    double temp_z = matrix->matrix[i][2];
    matrix->matrix[i][0] = cos(angle) * temp_x + sin(angle) * temp_z;
    matrix->matrix[i][2] = sin(angle) * (-temp_x) + cos(angle) * temp_z;
  }
}

void rotate_on_z_axis(matrix_t *matrix, double angle) {
  angle = angle * M_PI / 180;
  for (int i = 1; i < matrix->rows; i++) {
    double temp_x = matrix->matrix[i][0];
    double temp_y = matrix->matrix[i][1];
    matrix->matrix[i][0] = temp_x * cos(angle) + temp_y * sin(angle);
    matrix->matrix[i][1] = -temp_x * sin(angle) + temp_y * cos(angle);
  }
}

void move_on_x(matrix_t *matrix, double val) {
  for (int i = 1; i < matrix->rows; i++) {
    matrix->matrix[i][0] += val;
  }
}

void move_on_y(matrix_t *matrix, double val) {
  for (int i = 1; i < matrix->rows; i++) {
    matrix->matrix[i][1] += val;
  }
}

void move_on_z(matrix_t *matrix, double val) {
  for (int i = 1; i < matrix->rows; i++) {
    matrix->matrix[i][2] += val;
  }
}

int change_scale(matrix_t *matrix, double scale_val) {
  int err = NO_ERROR;
  if (scale_val != 0) {
    for (int i = 1; i < matrix->rows; i++) {
      matrix->matrix[i][0] *= scale_val;
      matrix->matrix[i][1] *= scale_val;
      matrix->matrix[i][2] *= scale_val;
    }
  } else {
    err = 1;
  }
  return err;
}

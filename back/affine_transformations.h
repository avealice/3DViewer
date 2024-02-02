#ifndef AFFINE_TRANSFORMATIONS_H
#define AFFINE_TRANSFORMATIONS_H

#include <math.h>

#include "parser.h"

void rotate_on_x_axis(matrix_t *matrix, double angle);
void rotate_on_y_axis(matrix_t *matrix, double angle);
void rotate_on_z_axis(matrix_t *matrix, double angle);
void move_on_x(matrix_t *matrix, double val);
void move_on_y(matrix_t *matrix, double val);
void move_on_z(matrix_t *matrix, double val);
int change_scale(matrix_t *matrix, double scale_val);

#endif // AFFINE_TRANSFORMATIONS_H

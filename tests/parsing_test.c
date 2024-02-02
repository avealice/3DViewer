#include <check.h>

#include "../back/affine_transformations.h"
#include "../back/parser.h"

#define EPS 1e-6

START_TEST(parser_err_1) {
  model dataModel;
  char filename[] = "obj_files/xxx.obj";
  int res = readFile(filename, &dataModel);
  ck_assert_int_eq(res, FILE_OPEN_ERROR);
}
END_TEST

START_TEST(parser_err_2) {
  model dataModel;
  char *filename = NULL;
  int res = readFile(filename, &dataModel);
  ck_assert_int_eq(res, FILE_OPEN_ERROR);
}
END_TEST

START_TEST(parser_cube_err) {
  model dataModel;
  char filename[] = "obj/cube_error.obj";
  int err = readFile(filename, &dataModel);
  ck_assert_int_eq(err, FILE_READ_ERROR);
  if (!err) modelDestructor(&dataModel);
}
END_TEST

START_TEST(parse_cube_vertices) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {-1, -1, -1},
      {-1, -1, 1},
      {-1, 1, -1},
      {-1, 1, 1}, 
      {1, -1, -1}, 
      {1, -1, 1},
      {1, 1, -1},
      {1, 1, 1}
  };
  int err = readFile(filename, &dataModel);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST


START_TEST(parse_cube_facets) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double polygons_arr[11][3] = {
      {0, 0, 0},
      {1, 2, 2},
      {1, 3, 7},
      {1, 4, 3},
      {1, 2, 4},
      {3, 8, 7}, 
      {3, 4, 8}, 
      {5, 7, 8},
      {5, 8, 6},
      {1, 5, 6}, 
      {1, 6, 2}
  };
  int err = readFile(filename, &dataModel);
  for (unsigned int i = 1; i <= dataModel.countFacets; i++) {
    for (int j = 0; j < dataModel.polygons[i].countVertexesInPolygon; j++) {
      ck_assert_int_eq(dataModel.polygons[i].vertexes[j], polygons_arr[i][j]);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST



START_TEST(rotate_x) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {-1, -1.158456, -0.81116},
      {-1, -0.811160, 1.158456},
      {-1, 0.811160, -1.158456},
      {-1, 1.158456, 0.811160},
      {1, -1.158456, -0.811160}, 
      {1,  -0.811160, 1.158456}, 
      {1, 0.811160, -1.158456},
      {1, 1.158456, 0.811160}
  };
  int err = readFile(filename, &dataModel);
  rotate_on_x_axis(&dataModel.matrixXYZ, 10);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    //printf("i %d\n", i);
    for (int j = 0; j < 3; j++) {
      //printf(" j %d\n", j);
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST

START_TEST(rotate_y) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {-1.158456, -1, -0.81116},
      {-0.811160, -1, 1.158456},
      {-1.158456, 1, -0.811160},
      {-0.811160, 1, 1.158456},
      {0.811160, -1, -1.158456}, 
      {1.158456,  -1, 0.811160}, 
      {0.811160, 1, -1.158456},
      {1.158456, 1, 0.811160}
  };
  int err = readFile(filename, &dataModel);
  rotate_on_y_axis(&dataModel.matrixXYZ, 10);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    //printf("i %d\n", i);
    for (int j = 0; j < 3; j++) {
      //printf(" j %d\n", j);
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST

START_TEST(rotate_z) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {-1.158456, -0.811160, -1.000000},
      {-1.158456, -0.811160, 1},
      {-0.811160, 1.158456, -1},
      {-0.811160, 1.158456, 1},
      {0.811160, -1.158456, -1}, 
      {0.811160,  -1.158456, 1}, 
      {1.158456, 0.811160, -1},
      {1.158456, 0.811160, 1}
  };
  int err = readFile(filename, &dataModel);
  rotate_on_z_axis(&dataModel.matrixXYZ, 10);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    //printf("i %d\n", i);
    for (int j = 0; j < 3; j++) {
      //printf(" j %d\n", j);
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST

START_TEST(translate_x) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {2, -1, -1},
      {2, -1, 1},
      {2, 1, -1},
      {2, 1, 1}, 
      {4, -1, -1}, 
      {4, -1, 1},
      {4, 1, -1},
      {4, 1, 1}
  };
  int err = readFile(filename, &dataModel);
  move_on_x(&dataModel.matrixXYZ, 3);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    //printf("i %d\n", i);
    for (int j = 0; j < 3; j++) {
      //printf(" j %d\n", j);
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST

START_TEST(translate_y) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {-1, 2, -1},
      {-1, 2, 1},
      {-1, 4, -1},
      {-1, 4, 1}, 
      {1, 2, -1}, 
      {1, 2, 1},
      {1, 4, -1},
      {1, 4, 1}
  };
  int err = readFile(filename, &dataModel);
  move_on_y(&dataModel.matrixXYZ, 3);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    //printf("i %d\n", i);
    for (int j = 0; j < 3; j++) {
      //printf(" j %d\n", j);
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST


START_TEST(translate_z) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {-1, -1, 2},
      {-1, -1, 4},
      {-1, 1, 2},
      {-1, 1, 4}, 
      {1, -1, 2}, 
      {1, -1, 4},
      {1, 1, 2},
      {1, 1, 4}
  };
  int err = readFile(filename, &dataModel);
  move_on_z(&dataModel.matrixXYZ, 3);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    //printf("i %d\n", i);
    for (int j = 0; j < 3; j++) {
      //printf(" j %d\n", j);
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST

START_TEST(scale) {
  model dataModel;
  char filename[] = "obj/cube.obj";
  double vertices_arr[9][3] = {
      {0, 0, 0},
      {-7, -7, -7},
      {-7, -7, 7},
      {-7, 7, -7},
      {-7, 7, 7}, 
      {7, -7, -7}, 
      {7, -7, 7},
      {7, 7, -7},
      {7, 7, 7}
  };
  int err = readFile(filename, &dataModel);
  change_scale(&dataModel.matrixXYZ, 7);
  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
    //printf("i %d\n", i);
    for (int j = 0; j < 3; j++) {
      //printf(" j %d\n", j);
      ck_assert_double_eq_tol(dataModel.matrixXYZ.matrix[i][j], vertices_arr[i][j], EPS);
    }
  }
  if (!err) modelDestructor(&dataModel);
}
END_TEST


Suite *lib_suite(void) {
//   Suite *s;
//   s = suite_create("Check");
//   TCase *tc_parser_1;
//   tc_parser_1 = tcase_create("parser_1");
//   suite_add_tcase(s, tc_parser_1);
//   tcase_add_test(tc_parser_1, parser_1);

//   TCase *tc_parser_2;
//   tc_parser_2 = tcase_create("parser_2");
//   suite_add_tcase(s, tc_parser_2);
//   tcase_add_test(tc_parser_2, parser_2);

//   TCase *tc_affine_rotate_X_1;
//   tc_affine_rotate_X_1 = tcase_create("affine_rotate_X_1");
//   suite_add_tcase(s, tc_affine_rotate_X_1);
//   tcase_add_test(tc_affine_rotate_X_1, affine_rotate_X_1);
Suite *s = suite_create("\033[0;1;36m3dViewer_V1.0\033[0m");
  TCase *tc = tcase_create("viewer");
  tcase_add_test(tc, parser_err_1);
  tcase_add_test(tc, parser_err_2);
  tcase_add_test(tc, parser_cube_err);
  tcase_add_test(tc, parse_cube_vertices);
  tcase_add_test(tc, parse_cube_facets);
  tcase_add_test(tc, rotate_x);
  tcase_add_test(tc, rotate_y);
  tcase_add_test(tc, rotate_z);
  tcase_add_test(tc, translate_x);
  tcase_add_test(tc, translate_y);
  tcase_add_test(tc, translate_z);
  tcase_add_test(tc, scale);
  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
//   Suite *s;
//   SRunner *sr;
//   s = lib_suite();
//   sr = srunner_create(s);
//   srunner_run_all(sr, CK_NORMAL);
//   srunner_free(sr);
//   return 0;
int failed = 0;
  Suite *suites_list[] = {lib_suite(), NULL};
  for (int i = 0; suites_list[i] != NULL; i++) {
    SRunner *sr = srunner_create(suites_list[i]);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
    printf("\n");
  }
  printf("\033[0;1;31mFAILED: %d\033[0m\n", failed);
  
  return failed == 0 ? 0 : 1;

}

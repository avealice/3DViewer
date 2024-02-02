#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ErrorCode {
  NO_ERROR,
  MEMORY_ALLOCATION_ERROR,
  FILE_OPEN_ERROR,
  FILE_READ_ERROR
};

typedef struct {
  double **matrix;
  int rows;
  int cols;
  double minX;
  double maxX;
  double minY;
  double maxY;
  double minZ;
  double maxZ;
} matrix_t;

typedef struct {
  int *vertexes;
  int countVertexesInPolygon;
} polygon_t;

typedef struct model {
  unsigned int countVertexes;
  unsigned int countFacets;
  matrix_t matrixXYZ;
  polygon_t *polygons;
} model;

int readFile(const char *file, model *dataModel);
int countVerticesAndFacets(FILE *file, model *dataModel);
void initData(model *dataModel);
void resetMatrix(matrix_t *matrix);
int createMatrixXYZ(int rows, int cols, matrix_t *result);
int parserFile(matrix_t *result, FILE *file, model *dataModel);
int countVertexesInPolygon(const char *str);
int addVertexToPolygon(int *arr, const char *str);
int isFacetsLine(char *row);
int isVertexLine(char *row);
int parseVertexLine(char *line, matrix_t *result, int current_vertex);
void centering(matrix_t *result);
void modelDestructor(model *dataModel);
void printModel(model dataModel);

#endif // PARSER_H

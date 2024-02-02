#include "parser.h"

// int main(void) {
//     model dataModel;
//     char *filename = "cube.obj";
//     int error = readFile(filename, &dataModel);
//     if (error == NO_ERROR) {
//         printModel(dataModel);
//     } else {
//         printf("%d\n", error);
//     }
//     return 0;
// }

int readFile(const char *file, model *dataModel) {
  // printf("ZASHEL!!!\n");
  int err = NO_ERROR;
  initData(dataModel);
  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    err = FILE_OPEN_ERROR;
    return err;
  }
  err = countVerticesAndFacets(fp, dataModel);
  if (err == NO_ERROR) {
    err =
        createMatrixXYZ(dataModel->countVertexes + 1, 3, &dataModel->matrixXYZ);
    dataModel->polygons =
        (polygon_t *)calloc(dataModel->countFacets + 1, sizeof(polygon_t));
    if (dataModel->polygons == NULL)
      err = MEMORY_ALLOCATION_ERROR;
  }
  if (err == NO_ERROR) {
    err = parserFile(&dataModel->matrixXYZ, fp, dataModel);
    if (err == NO_ERROR)
      centering(&dataModel->matrixXYZ);
  }
  fclose(fp);
  return err;
}

void initData(model *dataModel) {
  dataModel->countFacets = 0;
  dataModel->countVertexes = 0;
}

int isVertexLine(char *row) { return (row[0] == 'v' && row[1] == ' '); }

int isFacetsLine(char *row) { return (row[0] == 'f' && row[1] == ' '); }

int countVerticesAndFacets(FILE *file, model *dataModel) {
  int err = FILE_READ_ERROR;
  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, file) != -1) {
    if (isVertexLine(line))
      dataModel->countVertexes++;
    else if (isFacetsLine(line))
      dataModel->countFacets++;
  }
  if (dataModel->countFacets && dataModel->countVertexes)
    err = NO_ERROR;
  if (line)
    free(line);
  rewind(file);
  return err;
}

int createMatrixXYZ(int rows, int cols, matrix_t *result) {
  int error = MEMORY_ALLOCATION_ERROR;
  result->rows = rows;
  result->cols = cols;
  result->matrix = (double **)calloc(rows, sizeof(double *));
  if (result->matrix != NULL) {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)calloc(cols, sizeof(double));
    }
    error = NO_ERROR;
  }
  return error;
}

int parserFile(matrix_t *result, FILE *file, model *dataModel) {
  int err = NO_ERROR;
  size_t len = 0;
  char *line = NULL;
  int current_vertex = 1;
  int currentFacets = 1;
  while (getline(&line, &len, file) != -1 && err == NO_ERROR) {
    if (isVertexLine(line)) {
      err = parseVertexLine(line, result, current_vertex);
      current_vertex++;
    } else if (isFacetsLine(line)) {
      dataModel->polygons[currentFacets].countVertexesInPolygon =
          countVertexesInPolygon(line);
      dataModel->polygons[currentFacets].vertexes = (int *)calloc(
          dataModel->polygons[currentFacets].countVertexesInPolygon,
          sizeof(int));
      if (dataModel->polygons[currentFacets].vertexes != NULL) {
        err = addVertexToPolygon(dataModel->polygons[currentFacets].vertexes,
                                 line);
      } else {
        err = MEMORY_ALLOCATION_ERROR;
      }
      currentFacets++;
    }
  }
  if (line)
    free(line);
  rewind(file);
  return err;
}
int parseVertexLine(char *line, matrix_t *result, int current_vertex) {
  int err = NO_ERROR;
  double x, y, z;
  int ret = sscanf(line, "v %lf %lf %lf", &x, &y, &z);
  if (ret == 3) {
    if (current_vertex ==
        1) { // устанавливаем начальные минимальные и максимальные значения
      result->minX = result->maxX = x;
      result->minY = result->maxY = y;
      result->minZ = result->maxZ = z;
    }
    result->matrix[current_vertex][0] = x;
    result->matrix[current_vertex][1] = y;
    result->matrix[current_vertex][2] = z;
    if (x < result->minX)
      result->minX = x;
    if (x > result->maxX)
      result->maxX = x;
    if (y < result->minY)
      result->minY = y;
    if (y > result->maxY)
      result->maxY = y;
    if (z < result->minZ)
      result->minZ = z;
    if (z > result->maxZ)
      result->maxZ = z;
  } else {
    err = FILE_READ_ERROR;
  }
  return err;
}

int countVertexesInPolygon(const char *str) {
  int count = 0;
  int isToken = 0;
  for (int i = 1; str[i] != '\0'; i++) {
    if (str[i] == ' ') {
      isToken = 0;
    } else if (!isToken) {
      count++;
      isToken = 1;
    }
  }
  return count;
}

int addVertexToPolygon(int *arr, const char *str) {
  int err = NO_ERROR;
  str += 2; // Добавляем +2, чтобы пропустить 'f' и ' '
  const char *token = strtok((char *)str, " ");
  int current_index = 0;
  while (token != NULL && err == NO_ERROR) {
    char *slash = strchr(token, '/');
    if ('9' >= *token && *token >= '0') {
      if (slash == NULL) {
        int number = atoi(token);
        arr[current_index++] = number;
      } else { // Если у нас есть '/' в строке, то мы обрезаем строку до
               // указателя на первый '/'
        char *copy_token = strdup(token);
        int number = strtol(copy_token, &slash, 10);
        if (*copy_token == '-')
          err = FILE_READ_ERROR;
        arr[current_index++] = number;
        if (copy_token)
          free(copy_token);
      }
      token = strtok(NULL, " ");
    } else {
      err = FILE_READ_ERROR;
    }
  }
  return err;
}

//Центрируем нашу фигуру
void centering(matrix_t *result) {
  double centerX = (result->maxX + result->minX) / 2.0;
  double centerY = (result->maxY + result->minY) / 2.0;
  double centerZ = (result->maxZ + result->minZ) / 2.0;
  for (int i = 0; i < result->rows; i++) {
    result->matrix[i][0] -= centerX;
    result->matrix[i][1] -= centerY;
    result->matrix[i][2] -= centerZ;
  }
}

void modelDestructor(model *dataModel) {
  if (dataModel->polygons != NULL) {
    for (unsigned int i = 0; i < dataModel->countFacets; i++) {
      if (dataModel->polygons[i].vertexes != NULL) {
        free(dataModel->polygons[i].vertexes);
      }
    }
    free(dataModel->polygons);
  }
  if (dataModel->matrixXYZ.matrix != NULL) {
    for (int i = 0; i < dataModel->matrixXYZ.rows; i++) {
      free(dataModel->matrixXYZ.matrix[i]);
    }
    free(dataModel->matrixXYZ.matrix);
  }
  dataModel->countVertexes = 0;
  dataModel->countFacets = 0;
  dataModel->polygons = NULL;
  dataModel->matrixXYZ.matrix = NULL;
  dataModel->matrixXYZ.rows = 0;
  dataModel->matrixXYZ.cols = 0;
  dataModel->matrixXYZ.minX = 0.0;
  dataModel->matrixXYZ.maxX = 0.0;
  dataModel->matrixXYZ.minY = 0.0;
  dataModel->matrixXYZ.maxY = 0.0;
  dataModel->matrixXYZ.minZ = 0.0;
  dataModel->matrixXYZ.maxZ = 0.0;
}

// void printModel(model dataModel) {
//     for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
//             printf("vertex: %d x: %lf y: %lf z: %lf\n", i,
//             dataModel.matrixXYZ.matrix[i][0],
//             dataModel.matrixXYZ.matrix[i][1],
//             dataModel.matrixXYZ.matrix[i][2]);
//         }
//         printf("minX: %lf maxX: %lf\n", dataModel.matrixXYZ.minX,
//         dataModel.matrixXYZ.maxX); printf("minY: %lf maxY: %lf\n",
//         dataModel.matrixXYZ.minY, dataModel.matrixXYZ.maxY); printf("minZ:
//         %lf maxZ: %lf\n", dataModel.matrixXYZ.minZ,
//         dataModel.matrixXYZ.maxZ); for (unsigned int i = 1; i <
//         dataModel.countFacets + 1; i++) {
//             printf("polygon: %d facets: ", i);
//             for (int j = 0; j < dataModel.polygons[i].countVertexesInPolygon;
//             j++) {
//                 printf("%d ", dataModel.polygons[i].vertexes[j]);
//             }
//             printf("\n");
//         }
// }

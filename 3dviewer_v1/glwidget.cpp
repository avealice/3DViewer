#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget{parent} {}

//это просто инициализация, ниче сюда особо писать не надо
void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH);
}

//здесь пока просто пробно отрисовала куб, в этой функции происходит вся основная отрисовка
void GLWidget::paintGL()
{
    glClearColor(bg_red, bg_green, bg_blue, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (this->projection_type == 0) {
        glFrustum(-1 * normalize_coef, 1 * normalize_coef, -1 * normalize_coef, 1 * normalize_coef, normalize_coef, 1000 * normalize_coef);
        glTranslatef(0 ,0 , -2 * normalize_coef);
        glRotatef(30, 1, 0, 0);
    } else {
        glOrtho(-1 * normalize_coef, 1 * normalize_coef, -1 * normalize_coef, 1 * normalize_coef, -1 * normalize_coef, 1000 * normalize_coef);
        glTranslatef(0, -normalize_coef / 7.5, 0); // поменял с 2 на 7.5
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    if (this->vertices_style != 0) drawVertices();
    drawLines();
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::drawVertices(){
    if (this->vertices_style == 1) {
        glEnable(GL_POINT_SMOOTH);
    }
    glPointSize(this->vertices_size);
    glColor3f(this->v_red, this->v_green, this->v_blue);
    glBegin(GL_POINTS);
    for(int i = 1; i < dataModel.matrixXYZ.rows; i++){
           double x= dataModel.matrixXYZ.matrix[i][0];
           double y = dataModel.matrixXYZ.matrix[i][1];
           double z= dataModel.matrixXYZ.matrix[i][2];
           glVertex3d(x,y,z);
   }

   glEnd();
   if (this->vertices_style == 1) {
       glDisable(GL_POINT_SMOOTH);
   }
}

void GLWidget::drawLines() {
    if (this->edges_style == 1) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }
    glLineWidth(this->edges_thickness);
    glColor3f(this->e_red, this->e_green, this->e_blue);
    for (unsigned int i = 1; i <= dataModel.countFacets; i++) {
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < dataModel.polygons[i].countVertexesInPolygon; j++) {
            int vertexIndex = dataModel.polygons[i].vertexes[j];
            double x = dataModel.matrixXYZ.matrix[vertexIndex][0];
            double y = dataModel.matrixXYZ.matrix[vertexIndex][1];
            double z = dataModel.matrixXYZ.matrix[vertexIndex][2];
            glVertex3f(x, y, z);
        }
        glEnd();
    }
    if (this->edges_style == 1) {
        glDisable(GL_LINE_STIPPLE);
    }

}

void GLWidget::set_normalize_coef() {
  normalize_coef = -10;  // scarecrow

  for (int i = 1; i < dataModel.matrixXYZ.rows; i++) {
      for (size_t j = 0; j < 3; j++) {
        if (abs(dataModel.matrixXYZ.matrix[i][j]) > normalize_coef) {
            normalize_coef = abs(dataModel.matrixXYZ.matrix[i][j]);
        }
     }
  }
}


void GLWidget::takeScreenShot(const QString &fileName, const QString &format){

        makeCurrent();

        int width = this->width();
        int height = this->height();

        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            qDebug() << "Error: Framebuffer is not complete!";
            return;
        }

        glViewport(0, 0, width, height);
        paintGL();

        QImage image = grabFramebuffer();

        // Save the image in the specified format
        if (format.toLower() == "jpeg") {
            image.save(fileName, "JPEG");
        } else if (format.toLower() == "bmp") {
            image.save(fileName, "BMP");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &frameBuffer);
        glDeleteTextures(1, &texture);

        qDebug() << "Screenshot saved to" << fileName;
    }


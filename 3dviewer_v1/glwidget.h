#ifndef GLWIDGET_H
#define GLWIDGET_H
#define GL_SILENCE_DEPRECATION

#include <QOpenGLWidget>
#include <iostream>
#include <QDebug>
extern "C" {
#include "../back/parser.h"
#include "../back/affine_transformations.h"
}

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void drawVertices();
    void drawLines();
    void set_normalize_coef();

    void takeScreenShot(const QString &fileName, const QString &format);

public:
    const char *file;
    model dataModel = {};


    double scale_val = 50;
    double rotate_x = 0;
    double rotate_y = 0;
    double rotate_z = 0;
    double translate_x = 0;
    double translate_y = 0;
    double translate_z = 0;

    GLfloat normalize_coef;

    //SETTINGS
    int projection_type = 0;

    int vertices_style = 1;
    double vertices_size = 0;
    double v_red = 1, v_green = 1, v_blue = 1;

    int edges_style = 0;
    double edges_thickness = 1;
    double e_red = 1, e_green = 1, e_blue = 1;

    double bg_red = 0, bg_green = 0, bg_blue = 0;

};

#endif // GLWIDGET_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "glwidget.h"
#include "../back/affine_transformations.h"
#include <iostream>
#include <QDebug>
#include <QColorDialog>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageWriter>
#include <QTimer>
#include <QScreen>
#include <QList>
#include <QApplication>
#include <QThread>
#include <QMovie>
#include <qgifimage.h>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum projection_type { PARALLEL, CENTRAL };
    enum vertices_style { NONE, CIRCLE, SQUARE };
    enum edges_style { SOLID, DASHED };


    QGifImage *gif;
    QImage *frame;
    QTimer *timer;
    QSettings settings;// объявление настроек
    int count;


private slots:
    void on_pushButton_openFile_clicked();
    void on_pushButton_render_clicked();
    void on_slider_rotate_x_valueChanged(int value);
    void on_slider_rotate_y_valueChanged(int value);
    void on_slider_rotate_z_valueChanged(int value);
    void on_slider_scale_valueChanged(int value);
    void on_slider_translate_x_valueChanged(int value);
    void on_slider_translate_y_valueChanged(int value);
    void on_slider_translate_z_valueChanged(int value);
    void on_pushButton_vertices_clicked();
    void on_pushButton_edges_clicked();
    void on_pushButton_background_clicked();
    void on_radioButton_none_clicked();
    void on_radioButton_circle_clicked();
    void on_radioButton_square_clicked();
    void on_horizontalSlider_size_vert_valueChanged(int value);
    void on_pushButton_color_vertices_clicked();
    void on_pushButton_color_edges_clicked();
    void on_radioButton_solid_clicked();
    void on_radioButton_dashed_clicked();
    void on_horizontalSlider_thickness_valueChanged(int value);
    void on_comboBox_type_of_projection_currentIndexChanged(int index);
    void on_pushButton_background_color_clicked();
    void on_pushButton_screenshot_clicked();
    void on_pushButton_gif_clicked();

    void save();
    //настройки
    void loadLastSettings();
    void showEvent(QShowEvent *event);
    void saveSettings();

private:
    Ui::MainWindow *ui;
    void resetSliders();
};
#endif // MAINWINDOW_H

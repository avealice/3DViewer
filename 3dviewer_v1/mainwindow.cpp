#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    settings.beginGroup("3dViewerSettings");
    ui->setupUi(this);
    ui->comboBox_type_of_projection->addItem("Parallel");
    ui->comboBox_type_of_projection->addItem("Central");
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}


void MainWindow::on_pushButton_openFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "../objs", tr("Image Files (*.obj)"));
    if (!filePath.isEmpty()) {
        ui->lineEdit_path->setText(filePath);
    }

}

void MainWindow::saveSettings(){
   //сохранили стили кнопок
    settings.setValue("verticesBtn", ui->pushButton_vertices->styleSheet());
    settings.setValue("edgesBtn", ui->pushButton_edges->styleSheet());
    settings.setValue("backBtn", ui->pushButton_background->styleSheet());

    //background color of widget
    settings.setValue("lastRedBack", ui->GLwidget->bg_red);
    settings.setValue("lastGreenBack", ui->GLwidget->bg_green);
    settings.setValue("lastBlueBack", ui->GLwidget->bg_blue);
    settings.setValue("buttonColorCss", ui->pushButton_background_color->styleSheet());
    //vertices style
    settings.setValue("verticesStyle", ui->GLwidget->vertices_style);
//    qDebug()<<settings.value("verticesStyle").toInt()<<"in load";
    settings.setValue("radButtonNone", ui->radioButton_none->isChecked());
    settings.setValue("radButtonCircle", ui->radioButton_circle->isChecked());
    settings.setValue("radButtonSquare", ui->radioButton_square->isChecked());
    settings.setValue("vertSize", ui->horizontalSlider_size_vert->value());

    //edges style,rad buttons - checked status and slider value
    settings.setValue("radButtonSolid", ui->radioButton_solid->isChecked());
    settings.setValue("radButtonDashed", ui->radioButton_dashed->isChecked());
    settings.setValue("edgesStyle", ui->GLwidget->edges_style);
    //thickness slider
    settings.setValue("thicknessValue", ui->horizontalSlider_thickness->value());
    //motherfucking color
    settings.setValue("lastRedBackEdges", ui->GLwidget->e_red);
    settings.setValue("lastGreenBackEdges", ui->GLwidget->e_green);
    settings.setValue("lastBlueBackEdges", ui->GLwidget->e_blue);
    settings.setValue("buttonColorCssEdges", ui->pushButton_color_edges->styleSheet());
    // projection type
    settings.setValue("projType", ui->GLwidget->projection_type);
    settings.setValue("comboBoxVal", ui->comboBox_type_of_projection->currentText());
}

void MainWindow::loadLastSettings(){
//здесь загружаются последние настройкu
    ///// color of 3 buttons
    ui->pushButton_vertices->setStyleSheet(settings.value("verticesBtn").toString());
    ui->pushButton_edges->setStyleSheet(settings.value("edgesBtn").toString());
    ui->pushButton_background->setStyleSheet(settings.value("backBtn").toString());
     //background color of widget
    ui->GLwidget->bg_red = settings.value("lastRedBack").toDouble();
    ui->GLwidget->bg_green =settings.value("lastGreenBack").toDouble();
    ui->GLwidget->bg_blue = settings.value("lastBlueBack").toDouble();
    ui->pushButton_background_color->setStyleSheet(settings.value("buttonColorCss").toString());
    //vertices style, rad buttons - checked status and slider value
    ui->GLwidget->vertices_style = settings.value("verticesStyle").toInt();

    ui->radioButton_none->setChecked(settings.value("radButtonNone").toBool());
    ui->radioButton_circle->setChecked(settings.value("radButtonCircle").toBool());
    ui->radioButton_square->setChecked(settings.value("radButtonSquare").toBool());

    on_horizontalSlider_size_vert_valueChanged(settings.value("vertSize").toInt()); //это вызов функции
    ui->horizontalSlider_size_vert->setValue(settings.value("vertSize").toInt());
    //edges style,rad buttons - checked status and slider value
    ui->GLwidget->edges_style = settings.value("edgesStyle").toInt();
    ui->radioButton_solid->setChecked(settings.value("radButtonSolid").toBool());
    ui->radioButton_solid->setChecked(settings.value("radButtonDashed").toBool());
    //thickness value
    on_horizontalSlider_thickness_valueChanged(settings.value("thicknessValue").toInt());
    ui->horizontalSlider_thickness->setValue(settings.value("thicknessValue").toInt());
    //motherfucking color
    ui->GLwidget->e_red = settings.value("lastRedBackEdges").toDouble();
    ui->GLwidget->e_green =settings.value("lastGreenBackEdges").toDouble();
    ui->GLwidget->e_blue = settings.value("lastBlueBackEdges").toDouble();
    ui->pushButton_color_edges->setStyleSheet(settings.value("buttonColorCssEdges").toString());
    //projection type
    ui->GLwidget->projection_type = settings.value("projType").toInt();
    ui->comboBox_type_of_projection->setCurrentText(settings.value("comboBoxVal").toString());

}

void MainWindow::showEvent(QShowEvent *event){
    QMainWindow::showEvent(event);
    loadLastSettings();
}


//Здесь мы вызываем функцию парсинга, которая заполняет структуру и обновляем нашу отрисовку
//Записываем кол-во вершин и полигонов в label, тоже по заданию надо
void MainWindow::on_pushButton_render_clicked()
{
    if (!ui->lineEdit_path->text().isEmpty()) {
        modelDestructor(&ui->GLwidget->dataModel); //это очищаем всю структуру
        std::string std_filename = ui->lineEdit_path->text().toStdString();
        ui->GLwidget->file = (const char *)std_filename.c_str();
        int err = readFile(ui->GLwidget->file, &ui->GLwidget->dataModel);
        if (!err) {
            ui->GLwidget->set_normalize_coef();
            char *start = strrchr((char *)std_filename.c_str(), '/') + 1;
            char *ext = strrchr((char *)std_filename.c_str(), '.');
            *ext = '\0'; // это и две верхние строчки, чтобы название выводилось красиво
            ui->label_count_facets->setText(QString::number(ui->GLwidget->dataModel.countFacets));
            ui->label_count_vertexes->setText(QString::number(ui->GLwidget->dataModel.countVertexes));
            ui->label_name_file->setText(start);
            resetSliders();
            ui->GLwidget->update(); //это для opnegl чтобы обновилась отрисовка
        } else {
            // ошибка
        }
    } else {
        // ошибка
    }
}


void MainWindow::on_slider_rotate_x_valueChanged(int value)
{
    rotate_on_x_axis(&ui->GLwidget->dataModel.matrixXYZ, value - ui->GLwidget->rotate_x);
    ui->GLwidget->rotate_x = value;
    ui->GLwidget->update();
}


void MainWindow::on_slider_rotate_y_valueChanged(int value)
{
    rotate_on_y_axis(&ui->GLwidget->dataModel.matrixXYZ, value - ui->GLwidget->rotate_y);
    ui->GLwidget->rotate_y = value;
    ui->GLwidget->update();
}


void MainWindow::on_slider_rotate_z_valueChanged(int value)
{
    rotate_on_z_axis(&ui->GLwidget->dataModel.matrixXYZ, value - ui->GLwidget->rotate_z);
    ui->GLwidget->rotate_z = value;
    ui->GLwidget->update();
}


void MainWindow::on_slider_scale_valueChanged(int value)
{
    double val = (double)value / ui->GLwidget->scale_val;
    change_scale(&ui->GLwidget->dataModel.matrixXYZ, val);
    ui->GLwidget->scale_val = value;
    ui->GLwidget->update();
}


void MainWindow::on_slider_translate_x_valueChanged(int value)
{
    move_on_x(&ui->GLwidget->dataModel.matrixXYZ, (value - ui->GLwidget->translate_x) * ui->GLwidget->normalize_coef / 100);
    ui->GLwidget->translate_x = value;
    ui->GLwidget->update();
}


void MainWindow::on_slider_translate_y_valueChanged(int value)
{
    move_on_y(&ui->GLwidget->dataModel.matrixXYZ, (value - ui->GLwidget->translate_y) * ui->GLwidget->normalize_coef / 100);
    ui->GLwidget->translate_y = value;
    ui->GLwidget->update();
}


void MainWindow::on_slider_translate_z_valueChanged(int value)
{
    move_on_z(&ui->GLwidget->dataModel.matrixXYZ, (value - ui->GLwidget->translate_z) * ui->GLwidget->normalize_coef / 100);
    ui->GLwidget->translate_z = value;
    ui->GLwidget->update();
}

void MainWindow::resetSliders()
{
    ui->slider_scale->setValue(100);
    ui->slider_rotate_x->setValue(0);
    ui->slider_rotate_y->setValue(0);
    ui->slider_rotate_z->setValue(0);
    ui->slider_translate_x->setValue(0);
    ui->slider_translate_y->setValue(0);
    ui->slider_translate_z->setValue(0);
    ui->GLwidget->scale_val = 50;
    ui->GLwidget->rotate_x = 0;
    ui->GLwidget->rotate_y = 0;
    ui->GLwidget->rotate_z = 0;
    ui->GLwidget->translate_x = 0;
    ui->GLwidget->translate_y = 0;
    ui->GLwidget->translate_z = 0;
}


void MainWindow::on_pushButton_vertices_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_vertices->setStyleSheet("background-color: rgb(46, 95, 204)");
    ui->pushButton_edges->setStyleSheet("");
    ui->pushButton_background->setStyleSheet("");
}


void MainWindow::on_pushButton_edges_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->pushButton_edges->setStyleSheet("background-color: rgb(46, 95, 204)");
    ui->pushButton_vertices->setStyleSheet("");
    ui->pushButton_background->setStyleSheet("");

}


void MainWindow::on_pushButton_background_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->pushButton_background->setStyleSheet("background-color: rgb(46, 95, 204)");
    ui->pushButton_vertices->setStyleSheet("");
    ui->pushButton_edges->setStyleSheet("");

}


void MainWindow::on_radioButton_none_clicked()
{
    ui->GLwidget->vertices_style = NONE;
    ui->GLwidget->update();
}


void MainWindow::on_radioButton_circle_clicked()
{
    ui->GLwidget->vertices_style = CIRCLE;
    ui->GLwidget->update();
}


void MainWindow::on_radioButton_square_clicked()
{
    ui->GLwidget->vertices_style = SQUARE;
    ui->GLwidget->update();
}


void MainWindow::on_horizontalSlider_size_vert_valueChanged(int value)
{
    ui->GLwidget->vertices_size = value;
    ui->GLwidget->update();
}


void MainWindow::on_pushButton_color_vertices_clicked()
{
    QColor vert_color = QColorDialog::getColor(Qt::white, this, "Choose color");
      if (vert_color.isValid()) {
        ui->GLwidget->v_red = vert_color.redF();
        ui->GLwidget->v_green = vert_color.greenF();
        ui->GLwidget->v_blue = vert_color.blueF();
        char rgba_color[40];
        sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", vert_color.red(), vert_color.green(), vert_color.blue());
        ui->pushButton_color_vertices->setStyleSheet(rgba_color);
        ui->GLwidget->update();
      }
}


void MainWindow::on_pushButton_color_edges_clicked()
{
    QColor edge_color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (edge_color.isValid()) {
      ui->GLwidget->e_red = edge_color.redF();
      ui->GLwidget->e_green = edge_color.greenF();
      ui->GLwidget->e_blue = edge_color.blueF();
      char rgba_color[40];
      sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", edge_color.red(), edge_color.green(), edge_color.blue());
      ui->pushButton_color_edges->setStyleSheet(rgba_color);
      ui->GLwidget->update();
    }

}


void MainWindow::on_radioButton_solid_clicked()
{
    ui->GLwidget->edges_style = SOLID;
    ui->GLwidget->update();
}


void MainWindow::on_radioButton_dashed_clicked()
{
    ui->GLwidget->edges_style = DASHED;
    ui->GLwidget->update();
}


void MainWindow::on_horizontalSlider_thickness_valueChanged(int value)
{
    ui->GLwidget->edges_thickness = value / 10;
    ui->GLwidget->update();
}


void MainWindow::on_comboBox_type_of_projection_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->GLwidget->projection_type = PARALLEL;
    } else if (index == 1) {
        ui->GLwidget->projection_type = CENTRAL;
    }
    ui->GLwidget->update();
}


void MainWindow::on_pushButton_background_color_clicked()
{
    QColor bg_color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (bg_color.isValid()) {
      ui->GLwidget->bg_red = bg_color.redF();
      ui->GLwidget->bg_green = bg_color.greenF();
      ui->GLwidget->bg_blue = bg_color.blueF();
      char rgba_color[40];
      sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", bg_color.red(), bg_color.green(), bg_color.blue());
      ui->pushButton_background_color->setStyleSheet(rgba_color);
      ui->GLwidget->update();
    }

}




void MainWindow::on_pushButton_screenshot_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), "", tr("JPEG Files (*.jpg *.jpeg);;BMP Files (*.bmp)"));
        if (!filePath.isEmpty()) {
            QFileInfo fileInfo(filePath);
            QString fileExtension = fileInfo.suffix().toLower();

            if (fileExtension == "jpg" || fileExtension == "jpeg") {
                ui->GLwidget->takeScreenShot(filePath, "JPEG");
            } else if (fileExtension == "bmp") {
                ui->GLwidget->takeScreenShot(filePath, "BMP");
            } else {
                QMessageBox::warning(this, tr("Unsupported Format"), tr("Unsupported file format. Please select JPEG or BMP."));
            }
        }
}



void MainWindow::on_pushButton_gif_clicked()
{
    gif = new QGifImage;
    frame = new QImage;
    timer = new QTimer(this);
    count = 0;

    // Calculate the total number of frames and the timer interval
    int totalFrames = 200;
    int captureDurationMs = 5000; // 5 seconds
    int frameInterval = captureDurationMs / totalFrames;

    connect(timer, SIGNAL(timeout()), this, SLOT(save()));
    timer->start(frameInterval); // Use the calculated frame interval
}

void MainWindow::save() {
    count++;

    // Capture the frame with the GLWidget
    *frame = ui->GLwidget->grabFramebuffer();

// Resize the frame to the desired resolution (e.g., 640x480)
    *frame = frame->scaled(640, 480, Qt::KeepAspectRatio);
    gif->addFrame(*frame, 100);
    if (count == 200) { // Capture 200 frames
        timer->stop();
        QString save = QFileDialog::getSaveFileName(this, NULL, NULL, "GIF (*.gif)");
        if (!save.isNull()) {
            // Save the GIF
            gif->save(save);
        }
    }
}


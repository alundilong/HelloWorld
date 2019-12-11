#include "mainwindow.h"
#include <QApplication>
#include <QVTKOpenglWidget.h>
#include <QSurfaceFormat>
#include <vtkOpenGLRenderWindow.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "####################################";
    QApplication a(argc, argv);

    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(8); //1
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat()); //2

    MainWindow w;
    w.resize(700, 500);
    w.show();

    return a.exec();
}

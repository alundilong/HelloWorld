#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <QLineEdit>

class vtkVolumePlaneCutter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void signalLabelFile(const QString& dir, const QString &file);
    void signalVolumeFile(const QString &dir, const QString &file);
    void signalOutputFile(const QString &dir, const QString &file);

protected slots:
    void openLabelFile();
    void openOrigFile();
    void openOutputDir();

private:
    vtkVolumePlaneCutter *m_pCutter;
    QLineEdit *m_pDirEdit;
    QLineEdit *m_pDirEdit1;
    QLineEdit *m_pDirEdit2;
};

#endif // MAINWINDOW_H

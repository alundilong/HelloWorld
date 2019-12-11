#ifndef VTKVOLUMEPLANECUTTER_H
#define VTKVOLUMEPLANECUTTER_H

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <QString>

class vtkVolume;
class vtkGenericOpenGLRenderWindow;
class vtkPlaneWidget;
class vtkImageData;
class QString;
class vtkRenderer;

class vtkVolumePlaneCutter:public QVTKOpenGLWidget
{
    Q_OBJECT
public:
    vtkVolumePlaneCutter(QWidget *parent=nullptr);
    ~vtkVolumePlaneCutter();

    void setUpUi();

protected slots:
    void slotClipVolume();
    void slotLabelVolume(const QString&dir, const QString&file);
    void slotVolumeVolume(const QString&dir, const QString&file);
    void slotOutputDir(const QString&dir, const QString&file);

private:
    vtkSmartPointer<vtkVolume>          m_pVolume;
    vtkSmartPointer<vtkRenderWindow>    m_pRenderWindow;
    vtkSmartPointer<vtkPlaneWidget>     m_pPlaneWidget;
    vtkSmartPointer<vtkImageData>       m_pImageData;
    QString m_strLabelFile;
    QString m_strVolumeFile;
    QString m_strOutputFile;
    vtkSmartPointer<vtkRenderer> m_pRenderer;
};



#endif // VTKVOLUMEPLANECUTTER_H

#include <vtkAutoInit.h>
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

#include "vtkvolumeplanecutter.h"
#include "vtkImageData.h"
#include "vtkMetaImageReader.h"
#include "vtkRenderWindow.h"
#include "vtkPiecewiseFunction.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkColorTransferFunction.h"
#include "vtkCamera.h"
#include "vtkPlane.h"
#include "vtkPlaneWidget.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "qdebug.h"
#include "vtkImageData.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"
#include "vtkMetaImageWriter.h"
#include "vtkNIFTIImageReader.h"

vtkVolumePlaneCutter::vtkVolumePlaneCutter(QWidget*parent)
    : QVTKOpenGLWidget( parent )
    , m_pRenderer(nullptr)
{
    m_pRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_pRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_pRenderWindow->AddRenderer(m_pRenderer);
    this->SetRenderWindow(m_pRenderWindow);

    this->resize(640, 480);
}

vtkVolumePlaneCutter::~vtkVolumePlaneCutter()
{

}

void vtkVolumePlaneCutter::setUpUi()
{

}



void vtkVolumePlaneCutter::slotClipVolume()
{
  //进行裁剪
    vtkSmartPointer<vtkPlane> clippingPlane = vtkSmartPointer<vtkPlane>::New();
    m_pPlaneWidget->GetPlane( clippingPlane );

    vtkSmartPointer<vtkImageData> pImageData = vtkSmartPointer<vtkImageData>::New();

    double dblNormal[3], dblOrig[3];
    clippingPlane->GetNormal(dblNormal);
    clippingPlane->GetOrigin(dblOrig);

//    vtkSmartPointer<vtkImageData> pImageData = vtkSmartPointer<vtkImageData>::New();
//    vtkSmartPointer<vtkMetaImageReader> pLabelReader = vtkSmartPointer<vtkMetaImageReader>::New();
//    pLabelReader->SetFileName(m_strLabelFile.toStdString().c_str());
//    pLabelReader->Update();
//    pImageData = (vtkImageData*)pLabelReader->GetOutput();
    QString file = m_strLabelFile.mid(m_strLabelFile.lastIndexOf("/"));
    int n_pos = file.indexOf(".");
    QString file_ext = file.mid(n_pos);
    if( file_ext == "mhd" || file_ext == "mha"
            || file_ext == ".mhd" || file_ext == ".mha"){
        vtkSmartPointer<vtkMetaImageReader> pLabelReader;
        pLabelReader = vtkSmartPointer<vtkMetaImageReader>::New();
        pLabelReader->SetFileName(m_strLabelFile.toStdString().c_str());
        pLabelReader->Update();
        pImageData = (vtkImageData*)pLabelReader->GetOutput();
    }
    else if( file_ext == "nii.gz" || file_ext == "nii"
             || ".nii.gz" == file_ext || ".nii" == file_ext){
        vtkSmartPointer<vtkNIFTIImageReader> pLabelReader;
        pLabelReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
        pLabelReader->SetFileName(m_strLabelFile.toStdString().c_str());
        pLabelReader->Update();
        pImageData = (vtkImageData*)pLabelReader->GetOutput();
    }
    else
        qDebug() << "crap! not supported!" << file_ext ;

    int dims[3];
    pImageData->GetDimensions(dims);

    double center[3] = {(dims[0]-1)/2, (dims[1]-1)/2, (dims[2]-1)/2};
    double pCoords[3] = {0};

    // mask operation
    double d = -1*vtkMath::Dot(dblNormal, dblOrig);
    for (int i = 0;i<dims[0];++i){
        for (int j=0;j<dims[1];++j){
            for (int k=0;k<dims[2];++k){
                short*pLabel = (short*)pImageData->GetScalarPointer(i,j,k);
                if( *pLabel == 2){
                    for(int idx=0;idx<3;++idx){
                        pCoords[idx] = m_pImageData->GetOrigin()[idx];
                    }
                    pCoords[0] += (i - center[0])*m_pImageData->GetSpacing()[0];
                    pCoords[1] += (j - center[1])*m_pImageData->GetSpacing()[1];
                    pCoords[2] += (k - center[2])*m_pImageData->GetSpacing()[2];
                    double distance = vtkMath::Dot(dblNormal, pCoords) + d;
                    if (distance < 0)
                        continue;
                    else
                        *pLabel = 100;
                }
                short*pVoxel = (short*)m_pImageData->GetScalarPointer(i, j, k);
                if(*pLabel == 0 || *pLabel >= 100)
                    *pVoxel = -2048;
            }
        }
    }

    // original volume operation


    vtkSmartPointer<vtkMetaImageWriter> pWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
    pWriter->SetFileName(m_strOutputFile.toStdString().c_str());
    pWriter->SetInputData(m_pImageData);
    pWriter->SetCompression(false);
    pWriter->Write();

    m_pVolume->GetMapper()->AddClippingPlane( clippingPlane );
    m_pVolume->Modified();
    m_pRenderWindow->Modified();
    m_pRenderWindow->Render();
}

void vtkVolumePlaneCutter::slotLabelVolume(const QString &dir, const QString &file)
{
    m_strLabelFile = dir + file;

    vtkSmartPointer<vtkImageData> pImageData = vtkSmartPointer<vtkImageData>::New();

    int n_pos = file.indexOf(".");
    QString file_ext = file.mid(n_pos);
    if( file_ext == "mhd" || file_ext == "mha"
            || file_ext == ".mhd" || file_ext == ".mha"){
        vtkSmartPointer<vtkMetaImageReader> pLabelReader;
        pLabelReader = vtkSmartPointer<vtkMetaImageReader>::New();
        pLabelReader->SetFileName(m_strLabelFile.toStdString().c_str());
        pLabelReader->Update();
        pImageData = (vtkImageData*)pLabelReader->GetOutput();
    }
    else if( file_ext == "nii.gz" || file_ext == "nii"
             || ".nii.gz" == file_ext || ".nii" == file_ext){
        vtkSmartPointer<vtkNIFTIImageReader> pLabelReader;
        pLabelReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
        pLabelReader->SetFileName(m_strLabelFile.toStdString().c_str());
        pLabelReader->Update();
        pImageData = (vtkImageData*)pLabelReader->GetOutput();
    }
    else
        qDebug() << "crap! not supported!" << file_ext ;

    int dims[3];
    pImageData->GetDimensions(dims);

    // mask operation
    for (int i = 0;i<dims[0];++i){
        for (int j=0;j<dims[1];++j){
            for (int k=0;k<dims[2];++k){
                short*pLabel = (short*)pImageData->GetScalarPointer(i,j,k);
                short*pVoxel = (short*)m_pImageData->GetScalarPointer(i, j, k);
                if( *pLabel <= 0){
                    *pVoxel  = -2048;
                }
            }
        }
    }
    m_pImageData->Modified();
    m_pVolume->Modified();
    m_pRenderWindow->Modified();
    m_pRenderWindow->Render();
}

void vtkVolumePlaneCutter::slotVolumeVolume(const QString &dir, const QString &file)
{
    m_strVolumeFile = dir + file;
    qDebug() << m_strVolumeFile;

    m_pVolume = vtkSmartPointer<vtkVolume>::New();
    vtkSmartPointer<vtkMetaImageReader> pReader = vtkSmartPointer<vtkMetaImageReader>::New();
    pReader->SetFileName(m_strVolumeFile.toStdString().c_str());
    pReader->Update();

    m_pImageData = (vtkImageData*)pReader->GetOutputDataObject(0);

    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient( 0.4 );
    volumeProperty->SetDiffuse( 0.6 );
    volumeProperty->SetSpecular( 2 );

    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint( 0, 0.0 );
    compositeOpacity->AddPoint( 500, 0.15 );
    compositeOpacity->AddPoint( 1000, 0.15 );
    compositeOpacity->AddPoint( 1150, 0.85 );

    volumeProperty->SetScalarOpacity(compositeOpacity);

    vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    gradientOpacity->AddPoint( 0, 0.0 );
    gradientOpacity->AddPoint( 60, 0.6 );
    gradientOpacity->AddPoint( 100, 1. );
    volumeProperty->SetGradientOpacity(gradientOpacity);

    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0,    0.0, 0.0, 0.0);
    color->AddRGBPoint(500,  1.0, 0.5, 0.3);
    color->AddRGBPoint(1000, 1.0, 0.5, 0.3);
    color->AddRGBPoint(1150, 1.0, 1.0, 0.9);
    volumeProperty->SetColor( color );

    /*Volume*/
    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volumeMapper->SetInputData( m_pImageData );

    m_pVolume->SetMapper( volumeMapper );
    m_pVolume->SetProperty( volumeProperty );
    m_pVolume->SetOrigin( m_pVolume->GetCenter() );

    /*Render Window*/
    m_pRenderer->AddVolume( m_pVolume );
    m_pRenderer->ResetCamera();

    /*Clipping Plane Widget*/
    m_pPlaneWidget = vtkSmartPointer<vtkPlaneWidget>::New();
    m_pPlaneWidget->SetInteractor( m_pRenderWindow->GetInteractor() );
    m_pPlaneWidget->SetInputConnection( pReader->GetOutputPort() );
    m_pPlaneWidget->SetResolution( 500 );
    m_pPlaneWidget->GetPlaneProperty()->SetColor( .2, .8, 0.1 );
    m_pPlaneWidget->GetPlaneProperty()->SetOpacity( 0.5 );
    m_pPlaneWidget->GetHandleProperty()->SetColor( 0, .4, .7 );
    m_pPlaneWidget->GetHandleProperty()->SetLineWidth( 1.5 );
    m_pPlaneWidget->NormalToZAxisOn();
    m_pPlaneWidget->SetRepresentationToWireframe();
    m_pPlaneWidget->SetCenter( m_pVolume->GetCenter() );
    m_pPlaneWidget->PlaceWidget();
    m_pPlaneWidget->On();
}

void vtkVolumePlaneCutter::slotOutputDir(const QString &dir, const QString &file)
{
    m_strOutputFile = dir + file;
}

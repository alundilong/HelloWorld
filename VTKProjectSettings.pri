###########################
# To enable advanced dicom load, recompile VTK with vtkDICOM module checked,
# and uncomment the following line
#CONFIG   += ADVANCED_DICOM_LOADER
###########################
unix:!macx{
    INCLUDEPATH += ../../vtkbuilt/include/vtk-8.2

    LIBS += -L../../vtkbuilt/lib/
    LIBS += -lvtklz4-8.2 -lvtklzma-8.2 -lvtkdoubleconversion-8.2
}
mac*{

    INCLUDEPATH += /usr/local/include/vtk-8.2

    LIBS += -L/usr/local/lib/

}

win32{
    INCLUDEPATH += $$PWD/../../vtk8/include/vtk-8.2

    CONFIG(debug, debug|release){

        LIBS += -LD:/vtk-8.2.0-python-d/lib
        LIBS += -lDbghelp
        LIBS += -lvtkGUISupportQt-8.2 -lvtkCommonDataModel-8.2 -lvtkCommonMath-8.2 \
                -lvtksys-8.2 -lvtkCommonMisc-8.2 -lvtkCommonSystem-8.2 -lvtkCommonTransforms-8.2 \
                -lvtkFiltersExtraction-8.2 -lvtkFiltersCore-8.2 -lvtkFiltersGeneral-8.2 -lvtkCommonComputationalGeometry-8.2 \
                -lvtkFiltersStatistics-8.2 -lvtkImagingFourier-8.2 -lvtkImagingCore-8.2 -lvtkInteractionStyle-8.2 \
                -lvtkFiltersSources-8.2 -lvtkRenderingCore-8.2 -lvtkFiltersGeometry-8.2 -lvtkCommonExecutionModel-8.2 -lvtkCommonCore-8.2 -lvtkRenderingOpenGL2-8.2 \
                -lvtkImagingHybrid-8.2 -lvtkImagingStencil-8.2 -lvtkIOImage-8.2 -lvtkDICOMParser-8.2 -lvtkIOCore-8.2 -lvtkzlib-8.2 -lvtkmetaio-8.2 \
                -lvtkjpeg-8.2 -lvtkpng-8.2 -lvtktiff-8.2 -lvtkRenderingFreeType-8.2 -lvtkfreetype-8.2 \
                -lvtkInfovisCore-8.2 -lvtkViewsQt-8.2 -lvtkViewsInfovis-8.2 -lvtkCommonColor-8.2 -lvtkFiltersImaging-8.2 \
                -lvtkImagingGeneral-8.2 -lvtkImagingSources-8.2 -lvtkFiltersModeling-8.2 -lvtkInfovisLayout-8.2 \
                -lvtkRenderingContext2D-8.2 -lvtkRenderingLabel-8.2 -lvtkViewsCore-8.2 -lvtkInteractionWidgets-8.2 \
                -lvtkFiltersHybrid-8.2 -lvtkRenderingAnnotation-8.2 -lvtkImagingColor-8.2 -lvtkRenderingVolume-8.2 \
                -lvtkIOGeometry-8.2 -lvtkIOLegacy-8.2 -lvtkRenderingFreeType-8.2 -lvtkRenderingVolumeOpenGL2-8.2 \
                -lvtkglew-8.2 -lvtkImagingMath-8.2 -lvtkIOParallel-8.2
    }
    CONFIG(release, debug|release) {
         LIBS += -LD:/vtk-8.2.0-python-r/lib
         LIBS += -lvtkGUISupportQt-8.2 -lvtkCommonDataModel-8.2 -lvtkCommonMath-8.2 \
                -lvtksys-8.2 -lvtkCommonMisc-8.2 -lvtkCommonSystem-8.2 -lvtkCommonTransforms-8.2 \
                -lvtkFiltersExtraction-8.2 -lvtkFiltersCore-8.2 -lvtkFiltersGeneral-8.2 -lvtkCommonComputationalGeometry-8.2 \
                -lvtkFiltersStatistics-8.2 -lvtkImagingFourier-8.2 -lvtkImagingCore-8.2 -lvtkInteractionStyle-8.2 \
                -lvtkFiltersSources-8.2 -lvtkRenderingCore-8.2 -lvtkFiltersGeometry-8.2 -lvtkCommonExecutionModel-8.2 -lvtkCommonCore-8.2 -lvtkRenderingOpenGL2-8.2 \
                -lvtkImagingHybrid-8.2 -lvtkImagingStencil-8.2 -lvtkIOImage-8.2 -lvtkDICOMParser-8.2 -lvtkIOCore-8.2 -lvtkzlib-8.2 -lvtkmetaio-8.2 \
                -lvtkjpeg-8.2 -lvtkpng-8.2 -lvtktiff-8.2 -lvtkRenderingFreeType-8.2 -lvtkfreetype-8.2 \
                -lvtkInfovisCore-8.2 -lvtkViewsQt-8.2 -lvtkViewsInfovis-8.2 -lvtkCommonColor-8.2 -lvtkFiltersImaging-8.2 \
                -lvtkImagingGeneral-8.2 -lvtkImagingSources-8.2 -lvtkFiltersModeling-8.2 -lvtkInfovisLayout-8.2 \
                -lvtkRenderingContext2D-8.2 -lvtkRenderingLabel-8.2 -lvtkViewsCore-8.2 -lvtkInteractionWidgets-8.2 \
                -lvtkFiltersHybrid-8.2 -lvtkRenderingAnnotation-8.2 -lvtkImagingColor-8.2 -lvtkRenderingVolume-8.2 \
                -lvtkIOGeometry-8.2 -lvtkIOLegacy-8.2 -lvtkRenderingFreeType-8.2 -lvtkRenderingVolumeOpenGL2-8.2 \
                -lvtkglew-8.2 -lvtkImagingMath-8.2 -lvtkIOParallel-8.2
    }

    LIBS += -lAdvAPI32 -lOpenGL32 -luser32 -lgdi32 -lWbemUuid -lShell32 -ld3d9

}



contains(CONFIG, ADVANCED_DICOM_LOADER){
   DEFINES += ADVANCED_DICOM_LOADER_ENABLED
   LIBS += -lvtkDICOM-8.2 -lvtkDICOMParser-8.2 -lvtksqlite-8.2 -lvtkIOSQL-8.2
}

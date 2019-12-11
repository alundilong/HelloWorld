#include "mainwindow.h"
#include "vtkvolumeplanecutter.h"
#include "QHBoxLayout"
#include "qpushbutton.h"
#include "QtWidgets/qlineedit.h"
#include "QtWidgets/qlabel.h"
#include "qdebug.h"
#include "QFileDialog"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *pCentralWidget = new QWidget();



    QVBoxLayout *pVBLayout = new QVBoxLayout();
    pVBLayout->setSpacing(10);

    m_pCutter = new vtkVolumePlaneCutter();

    pVBLayout->addWidget(m_pCutter);
    pVBLayout->setStretchFactor(m_pCutter, 1);

    QVBoxLayout *pAnother= new QVBoxLayout();
    QVBoxLayout *pVtmp = new QVBoxLayout();
    QLabel *pLable = new QLabel("Select Volume:");
    QHBoxLayout *pHTmp = new QHBoxLayout();
    m_pDirEdit = new QLineEdit();
    QPushButton *pBrowseBtn = new QPushButton("Browse..");
    pHTmp->addWidget(m_pDirEdit); pHTmp->addWidget(pBrowseBtn);
    pVtmp->setSpacing(5);
    pVtmp->addWidget(pLable); pVtmp->addLayout(pHTmp);
    pAnother->addLayout(pVtmp);
    pCentralWidget->setLayout(pVBLayout);
    this->setCentralWidget(pCentralWidget);

    QVBoxLayout *pVtmp1 = new QVBoxLayout();
    pVtmp1->setSpacing(5);
    QLabel *pLable1 = new QLabel("Select Label Volume:");
    QHBoxLayout *pHTmp1 = new QHBoxLayout();
    m_pDirEdit1 = new QLineEdit();
    QPushButton *pBrowseBtn1 = new QPushButton("Browse..");
    pHTmp1->addWidget(m_pDirEdit1); pHTmp1->addWidget(pBrowseBtn1);
    pVtmp1->addWidget(pLable1); pVtmp1->addLayout(pHTmp1);
    pAnother->addLayout(pVtmp1);

    QVBoxLayout *pVtmp2 = new QVBoxLayout();

    QLabel *pLable2 = new QLabel("Select Output File:");
    QHBoxLayout *pHTmp2 = new QHBoxLayout();
    m_pDirEdit2 = new QLineEdit();
    QPushButton *pBrowseBtn2 = new QPushButton("Browse..");
    pHTmp2->addWidget(m_pDirEdit2); pHTmp2->addWidget(pBrowseBtn2);
    pVtmp2->addWidget(pLable2); pVtmp2->addLayout(pHTmp2);
    pAnother->addLayout(pVtmp2);

    pVBLayout->addLayout(pAnother);
    pVBLayout->setStretchFactor(pAnother, 0);
    QPushButton *pCutButton = new QPushButton("Cut Plane");
    pCutButton->setFixedSize(100, 30);
    pVBLayout->addWidget(pCutButton);


//    pCentralWidget->setLayout(pVBLayout);
//    this->setCentralWidget(pCentralWidget);

    QObject::connect( pCutButton, SIGNAL( clicked() ), m_pCutter, SLOT( slotClipVolume() ) );
    QObject::connect( pBrowseBtn, SIGNAL( clicked()), this, SLOT(openOrigFile()));
    QObject::connect( pBrowseBtn1, SIGNAL( clicked()), this, SLOT( openLabelFile()));
    QObject::connect( pBrowseBtn2, SIGNAL( clicked()), this, SLOT(openOutputDir()));

    QObject::connect( this, SIGNAL( signalLabelFile(const QString&,const QString&) ), m_pCutter, SLOT( slotLabelVolume(const QString&,const QString&)) );
    QObject::connect( this, SIGNAL( signalVolumeFile(const QString&,const QString&) ), m_pCutter, SLOT( slotVolumeVolume(const QString&, const QString&)) );
    QObject::connect( this, SIGNAL( signalOutputFile(const QString&, const QString&) ), m_pCutter, SLOT( slotOutputDir(const QString&,const QString&)) );

}

MainWindow::~MainWindow()
{

}

void MainWindow::openLabelFile()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Label Volume"), ".",
                tr("Volume Files (*.mha *.mhd *.nii.gz *.nii)"));
    if(path.length() > 0){
        QString dir={""}, file={""};
        m_pDirEdit1->setText(path);
        int dir_pos = path.lastIndexOf(QRegExp(QString("/")));
        if(dir_pos > -1)
        {
            dir = path.mid(0, dir_pos);
            file = path.mid(dir_pos);
        }
        emit signalLabelFile(dir, file);
    }
}

void MainWindow::openOrigFile()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Label Volume"), ".",
                tr("Volume Files (*.mha *.mhd)"));
    if(path.length() > 0){
        QString dir={""}, file={""};
        m_pDirEdit->setText(path);
        int dir_pos = path.lastIndexOf(QRegExp(QString("/")));
        if(dir_pos > -1)
        {
            dir = path.mid(0, dir_pos);
            file = path.mid(dir_pos);
        }
        emit signalVolumeFile(dir, file);
    }
}

void MainWindow::openOutputDir()
{
    QString path = QFileDialog::getSaveFileName(
                this, tr("Save Volume"), ".",
                tr("Volume Files (*.mhd)"));
    if(path.length() > 0){
        QString dir={""}, file={""};
        m_pDirEdit2->setText(path);
        int dir_pos = path.lastIndexOf(QRegExp(QString("/")));
        if(dir_pos > -1)
        {
            dir = path.mid(0, dir_pos);
            file = path.mid(dir_pos);
        }
        emit signalOutputFile(dir, file);
    }
}

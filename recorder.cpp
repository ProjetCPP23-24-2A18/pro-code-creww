#include "recorder.h"
#include "ui_recorder.h"
#include<QCamera>
#include<QCameraImageCapture>
#include<QCameraViewfinder>
#include<QBoxLayout>
#include<QMenu>
#include<QAction>
#include<QFileDialog>



recorder::recorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recorder)
{
    ui->setupUi(this);
    mCamera = new QCamera(this) ;
    mCameraViewfinder = new QCameraViewfinder(this);
    mCameraImageCapture = new QCameraImageCapture(mCamera,this);
    mLayout = new QVBoxLayout ;
    mOptionMenu = new QMenu("Options",this);
    mAllumerAction = new QAction("open camera",this);
    mEteindreAction = new QAction("close camera",this);
    mCaptureAction = new QAction("save",this);


    mOptionMenu->addActions({mAllumerAction,mEteindreAction,mCaptureAction});
    ui->OptionPushButton_2->setMenu(mOptionMenu);
    mCamera->setViewfinder(mCameraViewfinder);
    mLayout->addWidget(mCameraViewfinder);
    mLayout->setMargin(0);
    ui->scrollArea_2->setLayout(mLayout);

    connect(mAllumerAction, &QAction::triggered, [&] () {
    mCamera->start();
    });

    connect(mEteindreAction, &QAction::triggered,[&] () {
    mCamera->stop();
    });

    connect(mCaptureAction, &QAction::triggered,[&] () {
   auto filename = QFileDialog::getSaveFileName(this,"Enregistrer","/","Image(*.jpg;*.jpeg)");

   if (filename.isEmpty()){
     return;
    }
    mCameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QImageEncoderSettings ImageEncoderSettings;
    ImageEncoderSettings.setCodec("image/jpeg");
    ImageEncoderSettings.setResolution(1600,1200);
    mCameraImageCapture->setEncodingSettings(ImageEncoderSettings);
    mCamera->setCaptureMode(QCamera::CaptureStillImage);
    mCamera->start();
    mCamera->searchAndLock();
    mCameraImageCapture->capture(filename);
    mCamera->unlock();

    });
}


recorder::~recorder()
{
    delete ui;
}


#ifndef RECORDER_H
#define RECORDER_H

#include <QDialog>

namespace Ui {
class recorder;
}

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QVBoxLayout;
class QMenu;
class QAction;

class recorder : public QDialog
{
    Q_OBJECT

public:
    explicit recorder(QWidget *parent = nullptr);
    ~recorder();

    // Delete the copy constructor and assignment operator
    recorder(const recorder&) = delete;
    recorder& operator=(const recorder&) = delete;

private slots:


private:
    Ui::recorder *ui;
    QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mLayout;
    QMenu *mOptionMenu;
    QAction *mAllumerAction;
    QAction *mEteindreAction;
    QAction *mCaptureAction;
};

#endif // RECORDER_H

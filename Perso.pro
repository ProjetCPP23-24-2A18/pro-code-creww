QT       += core gui sql
QT += printsupport
QT += charts
QT += network
QT += multimedia
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT       += core gui sql multimedia multimediawidgets printsupport charts network
QT +=svg
QT +=serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ImageDelegate.cpp \
    arduino.cpp \
    arduinosk.cpp \
    chatserver.cpp \
    chatsocket.cpp \
    connection.cpp \
    dumessengerconnectiondialog.cpp \
    gestionpatient.cpp \
    main.cpp \
    mainwindow.cpp \
    notepad.cpp \
    patient.cpp \
    personnel.cpp \
    personnellist.cpp \
    qrcode.cpp \
    qrcodegeneratordemo.cpp \
    qrcodegeneratorworker.cpp \
    rdv.cpp \
    sms.cpp \
    smtp.cpp

HEADERS += \
    ImageDelegate.h \
    arduino.h \
    arduinosk.h \
    chatserver.h \
    chatsocket.h \
    connection.h \
    dumessengerconnectiondialog.h \
    gestionpatient.h \
    mainwindow.h \
    notepad.h \
    patient.h \
    personnel.h \
    personnellist.h \
    qrcode.h \
    rdv.h \
    sms.h \
    smtp.h

FORMS += \
    dumessengerconnectiondialog.ui \
    gestionpatient.ui \
    mainwindow.ui \
    notepad.ui \
    personnellist.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc

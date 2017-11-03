#-------------------------------------------------
#
# Project created by QtCreator 2017-10-18T13:54:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

TARGET = analyseContour
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    convolution.cpp \
    fonctions.cpp \
    contourHandler.cpp

HEADERS  += mainwindow.h \
    fonctions.hpp \
    convolution.hpp \
    cvimagewidget.h \
    contourhandler.h

FORMS    += mainwindow.ui

DISTFILES += \
    data/lena.jpg \
    data/cany.png \
    data/test.png

QT       += core gui

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

INCLUDEPATH += $$PWD/thrid-party/mysylar/mysylar  \
    $$PWD/thrid-party/mysylar/mysylar/thrid-party/yaml-cpp/include 


LIBS += -L"$$PWD/thrid-party/mysylar/lib"  \
    -L"$$PWD/thrid-party/yaml-cpp/build"  \
    -lmysylar \
    -lyaml-cpp 
    


SOURCES += \
    utils/ganconfig.cpp \
    utils/generatethread.cpp \
    imageview.cpp \
    main.cpp \
    mainwindow.cpp \
    testdialog.cpp \
    testwidge.cpp 

HEADERS += \
    utils/gan.h \
    utils/ganconfig.h \
    utils/generatethread.h \
    imageview.h \
    mainwindow.h \
    testdialog.h \
    testwidge.h \

FORMS += \
    imageview.ui \
    mainwindow.ui \
    testdialog.ui \
    testwidge.ui 

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += quick
CONFIG += c++11
QT += quickcontrols2
QT += multimedia multimediawidgets
QT += serialport

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TM2221-5/tm2221_5.cpp \
    main.cpp \
    controller.cpp \
    abstractsurface.cpp \
    samplequeue.cpp \
    settings.cpp \
    gstplayer.cpp \
    unitsmanager.cpp \
    ../CPP/log.cpp \
    ../CPP/global.cpp \
    ../CPP/Interfaces/udpserver.cpp \
    ../CPP/unitsparent.cpp \
    ../CPP/TM-2221-5/EM5215-2/em5215_2_struct.cpp \
    ../CPP/TM-2221-5/EM5215-2/em5215_2.cpp \
    ../CPP/TM-2221-5/VideoServer/videoserver_struct.cpp \
    ../CPP/TM-2221-5/VideoServer/videoserver.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

#INCLUDEPATH += ../Basler
INCLUDEPATH += $$PWD
INCLUDEPATH +=/usr/include/gstreamer-1.0
INCLUDEPATH +=/usr/include/glib-2.0
INCLUDEPATH +=/usr/lib/x86_64-linux-gnu/glib-2.0/include

CONFIG(debug, debug|release) { DESTDIR = ../../bin/debug } else { DESTDIR = ../../bin/release }

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    TM2221-5/tm2221_5.h \
    controller.h \
    abstractsurface.h \
    samplequeue.h \
    settings.h \
    unitsmanager.h \
    gstplayer.h \
    ../CPP/global.h \
    ../CPP/log.h \
    ../CPP/Interfaces/udpserver.h \
    ../CPP/TM-2221-5/EM5215-2/em5215_2.h \
    ../CPP/TM-2221-5/EM5215-2/em5215_2_struct.h \
    ../CPP/unitsparent.h \
    ../CPP/TM-2221-5/VideoServer/videoserver.h \
    ../CPP/TM-2221-5/VideoServer/videoserver_struct.h

unix|win32: LIBS += -lgobject-2.0

unix|win32: LIBS += -lgstreamer-1.0

unix|win32: LIBS += -lglib-2.0

unix|win32: LIBS += -lgstvideo-1.0

unix|win32: LIBS += -lgstapp-1.0


QT += quick
QT += concurrent
CONFIG += c++11
QT += quickcontrols2
QT += multimedia multimediawidgets
QT += serialport

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#The following define allows you to enable/disable messages in debug mode.
#Also whis define control GStreamer's Debug mode (using to make .dot and
#display GStreamers messages) - increases1. +2-3% CP
DEFINES += QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release) { DESTDIR = ../../bin/debug } else { DESTDIR = ../../bin/release }

INCLUDEPATH +=/opt/pylon5/include

#CPPFLAGS += $$system( /opt/pylon5/bin/pylon-config --cflags) -DUSE_GIGE

LIBS += $$system(/opt/pylon5/bin/pylon-config --libs-rpath)  $$system(/opt/pylon5/bin/pylon-config --libs)
INCLUDEPATH +=/usr/include/gstreamer-1.0
INCLUDEPATH +=/usr/include/glib-2.0
INCLUDEPATH +=/usr/lib/x86_64-linux-gnu/glib-2.0/include

SOURCES += \
    Pylon/cameraevents.cpp \
    Pylon/cameraimagehandler.cpp \
    Pylon/pyloncamera.cpp \
    Pylon/pyloncamera2.cpp \
    Pylon/pylonfeaturesenum.cpp \
    VideoTract/abstractsurface.cpp \
    VideoTract/gstplayer.cpp \
    VideoTract/samplequeue.cpp \
    main.cpp \
    settings.cpp \
    unitsmanager.cpp \
    TM2221-5/tm2221_5.cpp \
    ../CPP/global.cpp \
    ../CPP/log.cpp \
    ../CPP/Interfaces/blockcomport.cpp \
    ../CPP/Interfaces/udpclient.cpp \
    ../CPP/TM-2221-5/EM5215-2/em5215_2_struct.cpp \
    ../CPP/TM-2221-5/EM5215-2/em5215_2.cpp \
    ../CPP/TM-2221-5/EM5215-2/em5215_parser.cpp \
    ../CPP/unitsparent.cpp \
    ../CPP/TM-2221-5/VideoServer/videoserver_struct.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = design/plugins.qmltypes

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Pylon/cameraevents.h \
    Pylon/cameraimagehandler.h \
    Pylon/pyloncamera.h \
    Pylon/pylonfeaturesenum.h \
    VideoTract/abstractsurface.h \
    VideoTract/gstplayer.h \
    VideoTract/samplequeue.h \
    settings.h \
    unitsmanager.h \
    TM2221-5/tm2221_5.h \
    ../CPP/global.h \
    ../CPP/log.h \
    ../CPP/Interfaces/blockcomport.h \
    ../CPP/Interfaces/udpclient.h \
    ../CPP/TM-2221-5/EM5215-2/em5215_2_struct.h \
    ../CPP/TM-2221-5/EM5215-2/em5215_2.h \
    ../CPP/TM-2221-5/EM5215-2/em5215_parser.h \
    ../CPP/unitsparent.h \
    ../CPP/TM-2221-5/VideoServer/videoserver_struct.h

unix|win32: LIBS += -lglib-2.0

unix|win32: LIBS += -lgstreamer-1.0

unix|win32: LIBS += -lgobject-2.0

unix|win32: LIBS += -lgstvideo-1.0

unix|win32: LIBS += -lgstapp-1.0

unix|win32: LIBS += -lgobject-2.0


DISTFILES +=



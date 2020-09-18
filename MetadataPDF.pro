QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      =  MetadataPDF
TEMPLATE    =  app

SOURCES     += main.cpp\
               src/metadatapdf.cpp \
               framelesswindow/framelesswindow.cpp \
               framelesswindow/windowdragger.cpp


HEADERS     += src/metadatapdf.h \
               framelesswindow/framelesswindow.h \
               framelesswindow/windowdragger.h


FORMS       += ui/metadatapdf.ui \
               framelesswindow/framelesswindow.ui

RESOURCES   += resources/metadatapdf.qrc

######################################################################
# Automatically generated by qmake (3.1) Tue Feb 23 18:02:41 2021
######################################################################

TEMPLATE = app
TARGET = texstudio_poppler_crash_bug
INCLUDEPATH += .

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
poppler_qt_pkg = poppler-qt$${QT_MAJOR_VERSION}

CONFIG += link_pkgconfig

PKGCONFIG += $${poppler_qt_pkg}

INCLUDEPATH += src
DEPENDPATH += src


# Input
HEADERS += src/pdfrenderengine.h \
           src/pdfrenderengine_stress_t.h \
           src/pdfrendermanager.h
SOURCES += main.cpp \
           src/pdfrenderengine.cpp \
           src/pdfrenderengine_stress_t.cpp \
           src/pdfrendermanager.cpp
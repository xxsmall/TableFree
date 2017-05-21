CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(tablefreeplugin)
TEMPLATE    = lib

HEADERS     = tablefreeplugin.h \
    editmenu.h \
    editdialog.h \
    editdatadialog.h \
    areasplitdialog.h \
    dialog.h \
    inittabledialog.h
SOURCES     = tablefreeplugin.cpp \
    editmenu.cpp \
    editdialog.cpp \
    editdatadialog.cpp \
    areasplitdialog.cpp \
    dialog.cpp \
    inittabledialog.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(tablefree.pri)

FORMS += \
    editdialog.ui \
    editdatadialog.ui \
    areasplitdialog.ui \
    dialog.ui \
    inittabledialog.ui
#DESTDIR = /home/xscc/Qt5.6.0/Tools/QtCreator/lib/Qt/plugins/designer

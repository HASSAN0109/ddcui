#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T08:57:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ddcui
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

SOURCES += \
    feature_base_model.cpp \
    feature_item_model.cpp \
    feature_table_model.cpp \
    feature_value.cpp \
    feature_value_tableitem_abstract_editor.cpp \
    feature_value_tableitem_cb_editor.cpp \
    feature_value_tableitem_cont_editor.cpp \
    feature_value_tableitem_delegate.cpp \
    featureselectiondialog.cpp \
    main.cpp \
    mainwindow.cpp \
    monitor.cpp \
    vcplineitem.cpp \
    vcprequest.cpp \
    vcpthread.cpp 

#    c_feature_widget.cpp \

HEADERS += \
   ui_mainwindow2.h \
    ddcui_globals.h \
    feature_base_model.h \
    feature_item_model.h \
    feature_table_model.h \
    feature_value.h \
    feature_value_tableitem_abstract_editor.h \
    feature_value_tableitem_cb_editor.h \
    feature_value_tableitem_cont_editor.h \
    feature_value_tableitem_delegate.h \
    featureselectiondialog.h \
    mainwindow.h \
    misc.h \
    monitor.h \
    vcplineitem.h \
    vcprequest.h \
    vcpthread.h

#    c_feature_widget.h \
#   mainwindow.ui \

FORMS += \
    featureselectiondialog.ui

#    c_feature_widget.ui \

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += ddcutil

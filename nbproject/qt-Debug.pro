# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = ORE
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core
SOURCES += src/libtermkey-0.15b/driver-csi.c main.cpp src/libtermkey-0.15b/driver-ti.c src/libtermkey-0.15b/termkey.c src/OEditor.cpp
HEADERS += src/libtermkey-0.15b/termkey.h src/OEditor.h src/libtermkey-0.15b/termkey-internal.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += qextserialport-1.2beta2/src src 
LIBS += -largtable2 -lcurses   
include(qextserialport-1.2beta2/src/qextserialport.pri)

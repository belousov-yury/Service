#-------------------------------------------------
#
# Project created by QtCreator 2016-07-01T16:36:42
#
#-------------------------------------------------

QT       += core serialport sql network

QT       -= gui

OBJECTS_DIR = ./
DESTDIR = ../bin

TARGET = Service
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += 	../src/main.cpp \
			../src/uart.cpp \
			../src/modbus.cpp \
			../src/gpio.cpp \
			../src/dcunit.cpp \
			../src/device.cpp \
			../src/networkmanager.cpp\


HEADERS += \
			../src/uart.h \
			../src/modbus.h \
			../src/gpio.h \
			../src/dcunit.h \
			../src/device.h \
			../src/networkmanager.h

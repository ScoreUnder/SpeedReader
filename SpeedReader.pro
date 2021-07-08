#-------------------------------------------------
# Copyright 2014-2015 Vanniktech - Niklas Baudy
#
# This file is part of SpeedReader.
#
# SpeedReader is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# SpeedReader is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SpeedReader. If not, see <http://www.gnu.org/licenses/>.
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpeedReader
TEMPLATE = app
OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .rcc

VERSION = v1.4.0

DEFINES += APPLICATION_VERSION=\\\"$${VERSION}\\\"
DEFINES += APPLICATION_NAME=\\\"SpeedReader\\\"
DEFINES += ORGANIZATION_NAME=\\\"Vanniktech\\\"
DEFINES += ORGANIZATION_DOMAIN=\\\"http://www.vanniktech.de/\\\"

# Icons
win32{
    RC_FILE = SpeedReader.rc
}

mac {
    ICON = icon.icns
}

RESOURCES += resources.qrc

TRANSLATIONS = languages/en.ts \
               languages/de.ts \
               languages/cs.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    speedreadertext.cpp \
    settingswindow.cpp \
    settings.cpp\
    speedreadersegment.cpp \
    row.cpp \
    thread.cpp \
    i18n.cpp

HEADERS  += mainwindow.h \
    speedreadertext.h \
    lib/vntformatter.h \
    settingswindow.h \
    settings.h\
    speedreadersegment.h \
    row.h \
    thread.h \
    i18n.h \
    initializableqmap.h \
    speedreadertextdatasource.h

FORMS    += mainwindow.ui \
    settingswindow.ui

CONFIG += c++11

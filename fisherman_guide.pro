QT      += core gui sql
QT      += network
QT      += webengine
QT      += webenginewidgets
QT      += script
QT      += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS

VERSION = 7.7.7.7
QMAKE_TARGET_COMPANY = Zorin Co
QMAKE_TARGET_PRODUCT = FisherMan Guide
QMAKE_TARGET_DESCRIPTION = The best program in the world!!!
QMAKE_TARGET_COPYRIGHT = Zorin Evgeny

SOURCES += \
    classfish.cpp \
    completerID.cpp \
    database.cpp \
    forecast.cpp \
    jsonparser.cpp \
    main.cpp \
    mainwindow.cpp \
    weather.cpp \
    webclass.cpp

HEADERS += \
    classfish.h \
    completerID.h \
    database.h \
    forecast.h \
    jsonparser.h \
    mainwindow.h \
    weather.h \
    webclass.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    fisherman_guide_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    .gitignore \
    FishGuadeDB.db

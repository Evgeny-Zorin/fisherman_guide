QT      += core gui sql
QT      += network
QT      += webengine
QT      += webenginewidgets
QT      += script
QT      += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    classfish.cpp \
    database.cpp \
    main.cpp \
    mainwindow.cpp \
    webclass.cpp

HEADERS += \
    classfish.h \
    database.h \
    mainwindow.h \
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

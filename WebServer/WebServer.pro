QT -= gui
QT += core network sql

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TEMPLATE = app



HEADERS += \
    src/Controllers/CookieTestController.h \
    src/Controllers/DataBaseController.h \
    src/Controllers/HomeController.h \
    src/Controllers/LoginController.h \
    src/RequestMapper.h \
    src/global.h \
    src/userdatabase.h \
    src/Controllers/RegisterController.h \
    src/Controllers/AboutController.h

SOURCES += \
    src/Controllers/DataBaseController.cpp \
    src/Controllers/HomeController.cpp \
    src/RequestMapper.cpp \
    src/global.cpp \
    src/main.cpp \
    src/Controllers/LoginController.cpp \
    src/Controllers/CookieTestController.cpp \
    src/userdatabase.cpp \
    src/Controllers/RegisterController.cpp \
    src/Controllers/AboutController.cpp

OTHER_FILES += etc/*
include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/templateengine/templateengine.pri)




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

DISTFILES += \
    src/docroot/html_part_page/database.html \
    src/docroot/file/js/database1.js \
    src/docroot/html_part_page/register.html




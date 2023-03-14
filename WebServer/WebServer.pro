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
    src/Controllers/ListDataController.h \
    src/Controllers/LoginController.h \
    src/RequestMapper.h \
    src/global.h

SOURCES += \
    src/Controllers/DataBaseController.cpp \
    src/RequestMapper.cpp \
    src/global.cpp \
    src/main.cpp \
    src/Controllers/LoginController.cpp \
    src/Controllers/ListDataController.cpp \
    src/Controllers/CookieTestController.cpp

OTHER_FILES += etc/*
OTHER_FILES += src/docroot/page/*
include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/templateengine/templateengine.pri)




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/docroot/file/faviocon.ico \
    src/docroot/file/index.html \
    src/docroot/file/listdata.html \
    src/docroot/file/login.html \
    src/docroot/file/secondpage.html \
    src/docroot/file/style.css \
    src/docroot/html_part_page/end.dhtml \
    src/docroot/html_part_page/footer.dhtml \
    src/docroot/html_part_page/head.dhtml \
    src/docroot/html_part_page/header.dhtml \
    src/docroot/html_part_page/main_db.dhtml \
    src/docroot/html_part_page/main_login.dhtml \
    src/docroot/style.css





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -llibpq
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -llibpq
else:unix:!macx: LIBS += -L$$PWD/./ -llibpq

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

unix|win32: LIBS += -llibpq



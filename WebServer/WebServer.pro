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
OTHER_FILES += src/docroot/page/*
include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/templateengine/templateengine.pri)




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/docroot/file/Qt_icon.png \
    src/docroot/file/css/about.css \
    src/docroot/file/css/bootstrap-grid.css \
    src/docroot/file/css/bootstrap-grid.css.map \
    src/docroot/file/css/bootstrap-grid.min.css \
    src/docroot/file/css/bootstrap-grid.min.css.map \
    src/docroot/file/css/bootstrap-grid.rtl.css \
    src/docroot/file/css/bootstrap-grid.rtl.css.map \
    src/docroot/file/css/bootstrap-grid.rtl.min.css \
    src/docroot/file/css/bootstrap-grid.rtl.min.css.map \
    src/docroot/file/css/bootstrap-reboot.css \
    src/docroot/file/css/bootstrap-reboot.css.map \
    src/docroot/file/css/bootstrap-reboot.min.css \
    src/docroot/file/css/bootstrap-reboot.min.css.map \
    src/docroot/file/css/bootstrap-reboot.rtl.css \
    src/docroot/file/css/bootstrap-reboot.rtl.css.map \
    src/docroot/file/css/bootstrap-reboot.rtl.min.css \
    src/docroot/file/css/bootstrap-reboot.rtl.min.css.map \
    src/docroot/file/css/bootstrap-utilities.css \
    src/docroot/file/css/bootstrap-utilities.css.map \
    src/docroot/file/css/bootstrap-utilities.min.css \
    src/docroot/file/css/bootstrap-utilities.min.css.map \
    src/docroot/file/css/bootstrap-utilities.rtl.css \
    src/docroot/file/css/bootstrap-utilities.rtl.css.map \
    src/docroot/file/css/bootstrap-utilities.rtl.min.css \
    src/docroot/file/css/bootstrap-utilities.rtl.min.css.map \
    src/docroot/file/css/bootstrap.css \
    src/docroot/file/css/bootstrap.css.map \
    src/docroot/file/css/bootstrap.min.css \
    src/docroot/file/css/bootstrap.min.css.map \
    src/docroot/file/css/bootstrap.rtl.css \
    src/docroot/file/css/bootstrap.rtl.css.map \
    src/docroot/file/css/bootstrap.rtl.min.css \
    src/docroot/file/css/bootstrap.rtl.min.css.map \
    src/docroot/file/css/database.css \
    src/docroot/file/css/home.css \
    src/docroot/file/css/sign-in.css \
    src/docroot/file/js/bootstrap.bundle.js \
    src/docroot/file/js/bootstrap.bundle.js.map \
    src/docroot/file/js/bootstrap.bundle.min.js \
    src/docroot/file/js/bootstrap.bundle.min.js.map \
    src/docroot/file/js/bootstrap.esm.js \
    src/docroot/file/js/bootstrap.esm.js.map \
    src/docroot/file/js/bootstrap.esm.min.js \
    src/docroot/file/js/bootstrap.esm.min.js.map \
    src/docroot/file/js/bootstrap.js \
    src/docroot/file/js/bootstrap.js.map \
    src/docroot/file/js/bootstrap.min.js \
    src/docroot/file/js/bootstrap.min.js.map \
    src/docroot/file/politech_icon.PNG \
    src/docroot/file/postgresql_icon.png \
    src/docroot/html_part_page/end.html \
    src/docroot/html_part_page/footer.html \
    src/docroot/html_part_page/head.html \
    src/docroot/html_part_page/header.html \
    src/docroot/html_part_page/login.html \
    src/docroot/html_part_page/main_db.html \
    src/docroot/html_part_page/main_login.html \
    src/docroot/html_part_page/main_regdhtml \
    src/docroot/html_part_page/main_about.html \
    src/docroot/html_part_page/register.html



#unix:LIBS += -lpq
#win32:LIBS += libpqdll.lib

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../




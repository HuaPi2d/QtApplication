QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    subwidgets/submusicwidget.cpp \
    subwidgets/subvideowidget.cpp \
    subwindow/submusicwindow.cpp \
    subwindow/subvideowindow.cpp \
    universal/getnetwork.cpp \
    universal/rematcher.cpp

HEADERS += \
    mainwindow.h \
    subwidgets/submusicwidget.h \
    subwidgets/subvideowidget.h \
    subwindow/submusicwindow.h \
    subwindow/subvideowindow.h \
    universal/getnetwork.h \
    universal/rematcher.h

FORMS += \
    mainwindow.ui \
    subwidgets/submusicwidget.ui \
    subwidgets/subvideowidget.ui

INCLUDEPATH += \
    ../src

LIBS += \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

DEFINES += "STYLES_DIR=$$PWD/../styles"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lqtadvancedcss
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lqtadvancedcss
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lqtadvancedcss

RESOURCES += \
    resource.qrc

QT       += core gui network multimedia multimediawidgets quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    info/videoinfo.cpp \
    info/videoinfolistmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    otherwidgets/deletemusicdialog.cpp \
    otherwidgets/downloadlistwidget.cpp \
    otherwidgets/singledownloadframe.cpp \
    subwidgets/submusicwidget.cpp \
    subwidgets/subvideowidget.cpp \
    subwindow/submusicwindow.cpp \
    subwindow/subvideowindow.cpp \
    universal/getnetwork.cpp \
    universal/rematcher.cpp

HEADERS += \
    ../build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/app/ui_submusicwindow.h \
    customize/ScrollingLabel.h \
    customize/localmusiclistview.h \
    info/videoinfo.h \
    info/videoinfolistmodel.h \
    mainwindow.h \
    otherwidgets/deletemusicdialog.h \
    otherwidgets/downloadlistwidget.h \
    otherwidgets/singledownloadframe.h \
    subwidgets/submusicwidget.h \
    subwidgets/subvideowidget.h \
    subwindow/submusicwindow.h \
    subwindow/subvideowindow.h \
    universal/getnetwork.h \
    universal/rematcher.h \
    universal/requestpro.h

FORMS += \
    mainwindow.ui \
    otherwidgets/deletemusicdialog.ui \
    otherwidgets/downloadlistwidget.ui \
    otherwidgets/singledownloadframe.ui \
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

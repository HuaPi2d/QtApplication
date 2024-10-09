QT       += core gui network multimedia multimediawidgets quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QHotkey/qhotkey.cpp \
    QHotkey/qhotkey_win.cpp \
    customize/pixmappro.cpp \
    info/videoinfo.cpp \
    info/videoinfolistmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    otherwidgets/deletemusicdialog.cpp \
    otherwidgets/downloadlistwidget.cpp \
    otherwidgets/singledownloadframe.cpp \
    ssjjCore/ssjjCoreFun.cpp \
    subwidgets/randomlearningwidget.cpp \
    subwidgets/submusicwidget.cpp \
    subwidgets/subssjjwidget.cpp \
    subwidgets/subtexttransitionwidget.cpp \
    subwidgets/subvideowidget.cpp \
    subwindow/randomlearningwindow.cpp \
    subwindow/submusicwindow.cpp \
    subwindow/subssjjwindow.cpp \
    subwindow/subtexttransitionwindow.cpp \
    subwindow/subvideowindow.cpp \
    thread/entergamethread.cpp \
    thread/initializegamethread.cpp \
    thread/runscriptthread.cpp \
    thread/settlementthread.cpp \
    thread/ssjjmainthread.cpp \
    thread/startssjjthread.cpp \
    universal/findPicPro.cpp \
    universal/getnetwork.cpp \
    universal/programController.cpp \
    universal/rematcher.cpp \
    universal/snapShoot.cpp

HEADERS += \
    ../build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/app/ui_submusicwindow.h \
    QHotkey/QHotkey \
    QHotkey/qhotkey.h \
    QHotkey/qhotkey_p.h \
    customize/ScrollingLabel.h \
    customize/localmusiclistview.h \
    customize/pixmappro.h \
    customize/texteditpro.h \
    info/videoinfo.h \
    info/videoinfolistmodel.h \
    mainwindow.h \
    otherwidgets/deletemusicdialog.h \
    otherwidgets/downloadlistwidget.h \
    otherwidgets/singledownloadframe.h \
    ssjjCore/ssjjCoreFun.h \
    subwidgets/randomlearningwidget.h \
    subwidgets/submusicwidget.h \
    subwidgets/subssjjwidget.h \
    subwidgets/subtexttransitionwidget.h \
    subwidgets/subvideowidget.h \
    subwindow/randomlearningwindow.h \
    subwindow/submusicwindow.h \
    subwindow/subssjjwindow.h \
    subwindow/subtexttransitionwindow.h \
    subwindow/subvideowindow.h \
    thread/entergamethread.h \
    thread/initializegamethread.h \
    thread/runscriptthread.h \
    thread/settlementthread.h \
    thread/ssjjmainthread.h \
    thread/startssjjthread.h \
    universal/findPicPro.h \
    universal/getnetwork.h \
    universal/htmlParserPro.h \
    universal/mouseKeyboradControl.h \
    universal/programController.h \
    universal/rematcher.h \
    universal/requestpro.h \
    universal/snapShoot.h \
    universal/stringConvert.h

FORMS += \
    mainwindow.ui \
    otherwidgets/deletemusicdialog.ui \
    otherwidgets/downloadlistwidget.ui \
    otherwidgets/singledownloadframe.ui \
    subwidgets/randomlearningwidget.ui \
    subwidgets/submusicwidget.ui \
    subwidgets/subssjjwidget.ui \
    subwidgets/subtexttransitionwidget.ui \
    subwidgets/subvideowidget.ui

INCLUDEPATH += \
    ../src

INCLUDEPATH += D:\xml\include\libxml2

LIBS += \
    -LD:\xml\lib -llibxml2

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

LIBS += -LD:\qt-C++\Tools\mingw1120_64\x86_64-w64-mingw32\lib
win32: LIBS += -lwbemuuid

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../opencv/Qt/install/x64/mingw/lib/ -llibopencv_world480.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../opencv/Qt/install/x64/mingw/lib/ -llibopencv_world480.dll
else:unix: LIBS += -L$$PWD/../../../opencv/Qt/install/x64/mingw/ -llibopencv_world480.dll

INCLUDEPATH += $$PWD/../../../opencv/Qt/install/include
DEPENDPATH += $$PWD/../../../opencv/Qt/install/include

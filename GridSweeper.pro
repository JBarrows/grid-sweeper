QT       += core gui widgets

CONFIG += c++17

SOURCES += \
    board.cpp \
    ccbomb.cpp \
    ccfieldcell.cpp \
    ccmarker.cpp \
    ccrenderwidget.cpp \
    cellwidget.cpp \
    gridbuilder.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    board.h \
    ccbomb.h \
    ccfieldcell.h \
    ccmarker.h \
    ccrenderwidget.h \
    cellwidget.h \
    gridbuilder.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

GAMEGRID_PATH = $$OUT_PWD/../debug/GameGrid

# Importing library
LIBS += -L$$GAMEGRID_PATH/ -lGameGrid

INCLUDEPATH += $$PWD/../debug/GameGrid
DEPENDPATH += $$PWD/../debug/GameGrid

win32-g++: PRE_TARGETDEPS += $$GAMEGRID_PATH/libGameGrid.a
else:win32:!win32-g++: PRE_TARGETDEPS += $$GAMEGRID_PATH/GameGrid.lib
else:unix:!macx: PRE_TARGETDEPS += $$GAMEGRID_PATH/libGameGrid.a

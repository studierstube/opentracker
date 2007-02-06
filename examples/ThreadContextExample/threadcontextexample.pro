TEMPLATE = app

CONFIG += debug

FORMS = tctxmainwindow.ui

HEADERS = \
    ponghelp.h \
    tctxmainwindow.h \
    pongview.h \
    pongscene.h \
    posable.h \
    axes.h \
    goal.h \
    checkerfloor.h \
    arena.h \
    dilatedrect.h \
    playingfield.h \
    paddle.h \
    puck.h

SOURCES  = \
    main.cpp \
    tctxmainwindow.cpp \
    pongview.cpp \
    pongscene.cpp \
    posable.cpp \
    axes.cpp \
    goal.cpp \
    checkerfloor.cpp \
    arena.cpp \
    dilatedrect.cpp \
    paddle.cpp \
    playingfield.cpp \
    puck.cpp 

mac:INCLUDEPATH += /opt/local/include $(ACEROOT)
QMAKE_LIBS += -lopentracker
mac:QMAKE_LIBDIR += /opt/local/lib /sw/lib

mac:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused

TEMPLATE = app

CONFIG += debug

FORMS = tctxmainwindow.ui

HEADERS = \
    tctxmainwindow.h \
    pongview.h \
    pongscene.h \
    checkerfloor.h \
    arena.h \
    dilatedrect.h

SOURCES  = \
    main.cpp \
    tctxmainwindow.cpp \
    pongview.cpp \
    pongscene.cpp \
    checkerfloor.cpp \
    arena.cpp \
    dilatedrect.cpp

mac:INCLUDEPATH += /opt/local/include $(ACEROOT)
QMAKE_LIBS += -lopentracker
mac:QMAKE_LIBDIR += /opt/local/lib /sw/lib

mac:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused

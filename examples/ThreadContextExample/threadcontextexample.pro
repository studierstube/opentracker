TEMPLATE = app

CONFIG += debug

FORMS = tctxmainwindow.ui

HEADERS = \
    tctxmainwindow.h \
    pongview.h \
    pongscene.h

SOURCES  = \
    main.cpp \
    tctxmainwindow.cpp \
    pongview.cpp \
    pongscene.cpp

mac:INCLUDEPATH += /opt/local/include $(ACEROOT)
QMAKE_LIBS += -lopentracker
mac:QMAKE_LIBDIR += /opt/local/lib /sw/lib

mac:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused

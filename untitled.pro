TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += NON_MATLAB_PARSING
DEFINES += MAX_EXT_API_CONNECTIONS=255

SOURCES += \
        main.cpp \
        neuralnetwork.cpp \
        perceptron.cpp \
        remoteApi/extApi.c \
        remoteApi/extApiPlatform.c

HEADERS += \
    include/v_repConst.h \
    include/v_repLib.h \
    include/v_repTypes.h \
    neuralnetwork.hpp \
    perceptron.hpp \
    remoteApi/extApi.h \
    remoteApi/extApiInternal.h \
    remoteApi/extApiPlatform.h

win32 {
    LIBS += -lwinmm
    LIBS += -lWs2_32
}

DISTFILES += \
    left_data

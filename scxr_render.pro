QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

UI_DIR = ./ui
MOC_DIR = ./tmp
OBJECTS_DIR = ./tmp

CONAN_INCLUDEPATH += "/home/monkey/.conan/data/eigen/3.3.9/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include" \
    "/home/monkey/.conan/data/eigen/3.3.9/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include/eigen3"
CONAN_BUILDDIRS += "/home/monkey/.conan/data/eigen/3.3.9/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/"

CONAN_INCLUDEPATH_EIGEN += "/home/monkey/.conan/data/eigen/3.3.9/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include" \
    "/home/monkey/.conan/data/eigen/3.3.9/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include/eigen3"
CONAN_LIBS_EIGEN +=
CONAN_SYSTEMLIBS_EIGEN += -lm
CONAN_FRAMEWORKS_EIGEN +=
CONAN_FRAMEWORK_PATHS_EIGEN +=
CONAN_LIBDIRS_EIGEN +=
CONAN_BINDIRS_EIGEN +=
CONAN_RESDIRS_EIGEN +=
CONAN_BUILDDIRS_EIGEN += "/home/monkey/.conan/data/eigen/3.3.9/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/"
CONAN_DEFINES_EIGEN +=
CONAN_QMAKE_CXXFLAGS_EIGEN +=
CONAN_QMAKE_CFLAGS_EIGEN +=
CONAN_QMAKE_LFLAGS_EIGEN +=
CONAN_QMAKE_LFLAGS_EIGEN +=
CONAN_EIGEN_ROOT = "/home/monkey/.conan/data/eigen/3.3.9/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9"

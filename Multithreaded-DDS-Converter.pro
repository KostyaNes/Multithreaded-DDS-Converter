QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DDSConversion/Color32.cpp \
    DDSConversion/DDSFunc.cpp \
    DDSConversion/Jobs/BatchJob.cpp \
    DDSConversion/Jobs/ConvertImageJob.cpp \
    DDSConversion/Jobs/ImageConversionJobData.cpp \
    DDSConversion/Jobs/SaveTextureJob.cpp \
    DDSConversion/Jobs/SplitImageJob.cpp \
    DDSConversion/Textures/ImageTexel.cpp \
    DDSConversion/Textures/ImageUtils.cpp \
    DDSConversion/Textures/TextureBlock.cpp \
    DDSConversion/Textures/TextureData.cpp \
    JobManager/Job.cpp \
    JobManager/JobManager.cpp \
    JobManager/ThreadConfiguration.cpp \
    Widgets/MainWindow.cpp \
    main.cpp

HEADERS += \
    DDSConversion/Color32.h \
    DDSConversion/DDSDefines.h \
    DDSConversion/DDSFunc.h \
    DDSConversion/Jobs/BatchJob.h \
    DDSConversion/Jobs/ConvertImageJob.h \
    DDSConversion/Jobs/ImageConversionJobData.h \
    DDSConversion/Jobs/SaveTextureJob.h \
    DDSConversion/Jobs/SplitImageJob.h \
    DDSConversion/Textures/ImageTexel.h \
    DDSConversion/Textures/ImageUtils.h \
    DDSConversion/Textures/TextureBlock.h \
    DDSConversion/Textures/TextureData.h \
    JobManager/Job.h \
    JobManager/JobData.h \
    JobManager/JobManager.h \
    JobManager/ThreadConfiguration.h \
    ProjectDefines.h \
    Widgets/MainWindow.h

FORMS += \
    Widgets/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

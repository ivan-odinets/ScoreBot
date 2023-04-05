QT -= gui
QT += core sql network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = score-bot

DEFINES  += APP_NAME='\\"ScoreBot\\"'
DEFINES  += APP_VERSION='\\"0.2.0\\"'

#Uncomment this to disable delay in rolling. Can be used for debugging and/or testing purposes
#DEFINES += ENDLESS_ROLL

DESTDIR = ../bin
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
unix:OBJECTS_DIR = ../build/o/unix
win32:OBJECTS_DIR = ../build/o/win32
macx:OBJECTS_DIR = ../build/o/mac

CONFIG(release, debug|release) : DEFINES += QT_NO_DEBUG_OUTPUT

include(./QtTelegramBot/QtTelegramBot.pri)

HEADERS += \
    AppConfig.h \
    CommandLineParser.h \
    Database.h \
    DefaultUIStrings.h \
    ScoreBot.h \
    SuperRandomGenerator.h \
    UI.h

SOURCES += \
        AppConfig.cpp \
        CommandLineParser.cpp \
        Database.cpp \
        ScoreBot.cpp \
        SuperRandomGenerator.cpp \
        UI.cpp \
        main.cpp

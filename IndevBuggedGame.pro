TEMPLATE = app
CONFIG += c++17 strict_c++
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_RELEASE += -O2

win32 {
    LIBS += -L "C:\Program Files (x86)\SFML\lib"
    INCLUDEPATH += "C:\Program Files (x86)\SFML\include"
    DEPENDPATH += "C:\Program Files (x86)\SFML\include"
}

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

SOURCES += \
        block.cpp \
        blockManager.cpp \
        character.cpp \
        collision.cpp \
        editLevelState.cpp \
        event.cpp \
        eventManager.cpp \
        game.cpp \
        gamePlayed.cpp \
        gameState.cpp \
        gameStateStack.cpp \
        levelManager.cpp \
        main.cpp \
        mainMenuState.cpp \
        messageManager.cpp \
        messageState.cpp \
        movement.cpp \
        particle.cpp \
        particleManager.cpp \
        particleMotor.cpp \
        playState.cpp \
        playerPlay.cpp \
        screenTransitionState.cpp \
        startScreenState.cpp \
        utilities.cpp \
        versionNumber.cpp \
        widgetCursor.cpp \
        widgetText.cpp

HEADERS += \
    block.hpp \
    blockManager.hpp \
    character.hpp \
    collision.hpp \
    editLevelState.hpp \
    event.hpp \
    eventManager.hpp \
    game.hpp \
    gamePlayed.hpp \
    gameState.hpp \
    gameStateStack.hpp \
    global.hpp \
    levelManager.hpp \
    mainMenuState.hpp \
    messageManager.hpp \
    messageState.hpp \
    movement.hpp \
    particle.hpp \
    particleManager.hpp \
    particleMotor.hpp \
    playState.hpp \
    playerPlay.hpp \
    screenTransitionState.hpp \
    startScreenState.hpp \
    utilities.hpp \
    versionNumber.hpp \
    widgetCursor.hpp \
    widgetText.hpp

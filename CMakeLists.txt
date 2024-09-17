cmake_minimum_required(VERSION 3.5)

project(ChatClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Dialog/dialogsign.h Dialog/dialogsign.cpp Dialog/dialogsign.ui
        chat.h chat.cpp chat.ui
        Chats/listwidget.h Chats/listwidget.cpp
        Dialog/dialogsettingchat.h Dialog/dialogsettingchat.cpp Dialog/dialogsettingchat.ui
        Chats/listwidgetitem.h Chats/listwidgetitem.cpp
        resourse.qrc
        Dialog/dialogchoosesmile.h Dialog/dialogchoosesmile.cpp Dialog/dialogchoosesmile.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ChatClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ChatClient APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ChatClient SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ChatClient
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ChatClient PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ChatClient PRIVATE Qt${QT_VERSION_MAJOR}::Network)

if (${BUILD_WINDOWS})
    target_link_libraries(ChatClient PUBLIC "/home/dmitry/dev/Libs/bass24-win/x64/bass.dll")
else()
#    target_link_libraries(ChatClient PUBLIC "/home/dmitry/dev/Libs/bass24-linux/libs/x86_64/libbass.so")
endif()

set_target_properties(ChatClient PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ChatClient)
endif()

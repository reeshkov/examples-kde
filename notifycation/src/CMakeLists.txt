cmake_minimum_required(VERSION 3.16)

project(notifycation LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt5 CONFIG REQUIRED
    Core
    Widgets
)

find_package(KF5Notifications REQUIRED)

#include_directories(/usr/include/qt5)
#include_directories(/usr/include/qt5/QtCore)
#include_directories(/usr/include/qt5/QtGui)
#include_directories(/usr/include/KF5/KNotifications)
#include_directories(/usr/include/KF5/KCoreAddons)
#include_directories(/usr/include/KF5/KI18n)

#add_definitions(-DQT_QML_DEBUG)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_executable(${CMAKE_PROJECT_NAME} "main.cpp")

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PRIVATE
    Qt5::Widgets
    KF5::Notifications
)
# qt5_use_modules(${CMAKE_PROJECT_NAME} Widgets Quick DBus)

file(WRITE "${CMAKE_PROJECT_NAME}.notifyrc" "[Global]
Name=${CMAKE_PROJECT_NAME}
Action=Taskbar|Sound|Popup|Logfile|TTS")

#include(FeatureSummary)
#find_package(ECM 5.97.0 NO_MODULE)
#set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")

#include(KDEInstallDirs)
#install(FILES "${CMAKE_PROJECT_NAME}.notifyrc"  DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})

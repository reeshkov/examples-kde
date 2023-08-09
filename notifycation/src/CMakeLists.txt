cmake_minimum_required(VERSION 3.16)

project(notifytest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt5Core CONFIG REQUIRED)

find_package(KF5Notifications REQUIRED)

find_package(KF5JobWidgets REQUIRED)

find_package(ECM REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

add_executable(${CMAKE_PROJECT_NAME} "main.cpp")

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PRIVATE
    KF5::Notifications
    KF5::JobWidgets
)


include(KDEInstallDirs)
message("KDE_INSTALL_TARGETS_DEFAULT_ARGS=${KDE_INSTALL_TARGETS_DEFAULT_ARGS}")

install(TARGETS ${CMAKE_PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

file(WRITE "${CMAKE_PROJECT_NAME}.notifyrc" "[Global]
Name=${CMAKE_PROJECT_NAME}
Action=Taskbar|Sound|Popup|Logfile|TTS
[Event/test1]
Name=Test_1
Comment=Test 1
#Action=Taskbar|Sound|Popup|Logfile|TTS
Action=Popup|Logfile
")
message("KDE_INSTALL_KNOTIFYRCDIR=${KDE_INSTALL_KNOTIFYRCDIR}")
install(FILES "${CMAKE_PROJECT_NAME}.notifyrc"  DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})

file(WRITE "${CMAKE_PROJECT_NAME}.desktop" "[Desktop Entry]
Name=Notify job test
Comment=Notify job testing
GenericName=Notify job test
Exec=${CMAKE_PROJECT_NAME}
Icon=system-file-manager
Type=Application
StartupNotify=true
Categories=Tests;
Keywords=jobnotify;
")
message("KDE_INSTALL_APPDIR=${KDE_INSTALL_APPDIR}")
install (FILES ${CMAKE_PROJECT_NAME}.desktop DESTINATION ${KDE_INSTALL_APPDIR})

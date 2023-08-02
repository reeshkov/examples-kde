cmake_minimum_required(VERSION 3.16)

project(notifytest LANGUAGES CXX)

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

add_executable(${CMAKE_PROJECT_NAME} "main.cpp")

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PRIVATE
    Qt5::Widgets
    KF5::Notifications
)

file(WRITE "${CMAKE_PROJECT_NAME}.notifyrc" "[Global]
Name=${CMAKE_PROJECT_NAME}
Action=Taskbar|Sound|Popup|Logfile|TTS
[Event/test1]
Name=Test_1
Comment=Test 1
#Action=Taskbar|Sound|Popup|Logfile|TTS
Action=Popup|Logfile
")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})
include(KDEInstallDirs)
install(FILES "${CMAKE_PROJECT_NAME}.notifyrc"  DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})

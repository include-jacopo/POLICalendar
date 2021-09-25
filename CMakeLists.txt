cmake_minimum_required(VERSION 3.20)
project(POLICalendar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(LibIcal REQUIRED)
include_directories(${LibIcal_INCLUDE_DIR})

add_executable(POLICalendar main.cpp
        View/MainWindow.cpp View/MainWindow.h
        View/Login/Login.cpp View/Login/Login.h
        View/Calendar/CalendarDate.cpp View/Calendar/CalendarDate.h
        View/Calendar/Calendar.cpp View/Calendar/Calendar.h
        View/Calendar/HourVBar.cpp View/Calendar/HourVBar.h
        View/Calendar/CalendarEvent.cpp View/Calendar/CalendarEvent.h
        View/Calendar/CalendarEvents.cpp View/Calendar/CalendarEvents.h
        View/Calendar/CalendarColumns.cpp View/Calendar/CalendarColumns.h
        View/Calendar/Tasklist.cpp View/Calendar/Tasklist.h
        View/Calendar/TaskGUI.cpp View/Calendar/TaskGUI.h
        View/Calendar/DialogEdit.cpp View/Calendar/DialogEdit.h
        Model/Event.cpp Model/Event.h
        Model/Task.cpp Model/Task.h
        Controller/Controller.cpp Controller/Controller.h
        Controller/WebClient.cpp Controller/WebClient.h
        Controller/Base64.h
        Controller/XMLReader.cpp Controller/XMLReader.h
        Controller/IcalHandler.cpp Controller/IcalHandler.h
        View/IView.h Controller/IController.h
        View/Calendar/ICalendarGUIEventsHandler.h)

target_link_libraries(POLICalendar
        Qt::Core
        Qt::Gui
        Qt::Widgets)
target_link_libraries(POLICalendar ${LibIcal_LIBRARIES})
IF(APPLE)
    find_library(NEON neon)
    target_link_libraries(POLICalendar ${NEON})
    find_library(PUGIXML pugixml)
    target_link_libraries(POLICalendar ${PUGIXML})
ELSE()
    target_link_libraries(POLICalendar neon)
    target_link_libraries(POLICalendar pugixml)
ENDIF()


if (WIN32)
    set(DEBUG_SUFFIX)
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

cmake_minimum_required(VERSION 3.16)

project(TestDatabases VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Шляхи до MySQL Connector/C (замініть на реальні шляхи)
set(MYSQL_INCLUDE_DIR "C:/Program Files/MySQL/mysql-8.0.39-winx64/include")
set(MYSQL_LIB_DIR "C:/Program Files/MySQL/mysql-8.0.39-winx64/lib")

# Додаємо шляхи до MySQL
include_directories(${MYSQL_INCLUDE_DIR})
link_directories(${MYSQL_LIB_DIR})

# Пошук компонентів Qt5 або Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Список файлів проєкту
set(PROJECT_SOURCES
        main.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TestDatabases
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        TMiscUtils.cpp
        TMiscUtils.h
    )
else()
    if(ANDROID)
        add_library(TestDatabases SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TestDatabases
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Лінкуємо бібліотеки Qt6 або Qt5
target_link_libraries(TestDatabases PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TestDatabases PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

# Лінкуємо MySQL бібліотеку
target_link_libraries(TestDatabases PRIVATE "${MYSQL_LIB_DIR}/libmysql.dll")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TestDatabases)
endif()
set_target_properties(TestDatabases PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TestDatabases
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TestDatabases)
endif()

cmake_minimum_required(VERSION 3.16)

project(untitled1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(HEADERS
    mainwindow.h
    C:/Users/User/Documents/test_untitled1/tst_db_productivity.h    # Указываем абсолютный путь к заголовочному файлу
)

set(TS_FILES untitled1_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        #"C:/Users/User/Documents/tests_untitled1/tst_db_productivity.cpp"
        mainwindow.ui
        ${TS_FILES}
)
set(TEST_SOURCES
    C:/Users/User/Documents/tests_untitled1/tst_db_productivity.cpp
)
add_executable(db_productivity_test
    ${TEST_SOURCES}
)
target_link_libraries(db_productivity_test
    PRIVATE Qt${QT_VERSION_MAJOR}::Test
    Qt6::Sql
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(untitled1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        db_manager.h db_manager.cpp
        registrationwindow.h registrationwindow.cpp registrationwindow.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(untitled1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(untitled1
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
include_directories("C:/Users/User/Documents/untitled1/include")
link_directories("C:/Users/User/Documents/untitled1/include/lib")


target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt6::Core
    Qt6::Sql
    Qt6::Gui
    Qt6::Test
    Qt6::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.untitled1)
endif()
set_target_properties(untitled1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS untitled1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled1)
endif()

enable_testing()
add_test(NAME db_productivity_test COMMAND db_productivity_test)

cmake_minimum_required(VERSION 3.19)
project(QuickChat LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# ✅ Find Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Network)

qt_add_executable(QuickChat
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    setup_db.h
    database_manager.h database_manager.cpp
    view_pvt_chat.h view_pvt_chat.cpp
    view_group_chat.h view_group_chat.cpp
    main_menu.h main_menu.cpp
    group_chat_list.h group_chat_list.cpp
)

# Copy logo.png to the build directory
configure_file(logo.png ${CMAKE_BINARY_DIR}/logo.png COPYONLY)

target_link_libraries(QuickChat
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt${QT_VERSION_MAJOR}::Sql  # Linking QtSql for SQLite
        Qt::Network
)

include(GNUInstallDirs)

install(TARGETS QuickChat
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QuickChat
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

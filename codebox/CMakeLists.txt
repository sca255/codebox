cmake_minimum_required(VERSION 3.16)
project(codebox LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
)
qt_standard_project_setup()
#find_package(curl REQUIRED COMPONENTS ALL)
find_package(RocksDB CONFIG REQUIRED)
find_package(CURL REQUIRED)
set(PROJECT_SOURCES
    main.cpp
)
add_definitions(-DCURL_STATICLIB)

set(
    curllib "${CMAKE_SOURCE_DIR}/codebox/lib/libcurl_a.lib" 
)
qt_add_executable(codebox ${PROJECT_SOURCES})


target_link_libraries(codebox
    PUBLIC
        Qt::Core
        RocksDB::rocksdb
    PRIVATE CURL::libcurl
     
)

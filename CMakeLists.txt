cmake_minimum_required(VERSION 3.5)

project(ThreadCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)

set(SOURCE_FILES
    main.cpp
    convertor.cpp
    thread.cpp
    id.cpp
    debug.cpp
    argumentparser.cpp
    troutputstream.cpp
    trinputstream.cpp
    interface.cpp
    system.cpp
    ifinputstream.cpp
    ifoutputstream.cpp
    ifsystem.cpp
    room.cpp
    module.cpp
    package.cpp
    pkinputstream.cpp
    pkoutputstream.cpp
    datapack.cpp)
#    jsonloader.cpp
#    jsonsaver.cpp)

set(INCLUDE_FILES
    sortedlist.h
    convertor.h
    thread.h
    id.h
    debug.h
    argumentparser.h
    troutputstream.h
    trinputstream.h
    interface.h
    system.h
    ifinputstream.h
    ifoutputstream.h
    enums.h
    ifsystem.h
    room.h
    pool.h
    module.h
    package.h
    pkinputstream.h
    pkoutputstream.h
    defines.h
    datapack.h)
#    jsonloader.h
#    jsonsaver.h)

add_executable(ThreadCore ${SOURCE_FILES} ${INCLUDE_FILES})

target_link_libraries(ThreadCore
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Threads::Threads)

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

set(OPENCV_INCLUDES /home/aaiwprton/.local_builds/opencv/include/opencv4/)
set(OPENCV_LIBS /home/aaiwprton/.local_builds/opencv/lib/)

include_directories(${OPENCV_INCLUDES})
link_directories(${OPENCV_LIBS})

set(SOURCES
    main.cpp)
set(SOURCE_CORE
    Core/system.cpp
    Core/package.cpp
    Core/interface.cpp
    Core/thread.cpp
    Core/module.cpp)
set(SOURCE_DATAPACKS
    )
set(SOURCE_INTERFACES
    )
set(SOURCE_META
    )
set(SOURCE_THREADS
    )
set(SOURCE_UTILS
    Utils/idgen.cpp)
set(SOURCE_MODULES
    Modules/md_iosystem.cpp
    Modules/md_lfreader.cpp
    Modules/pk_iosystem.cpp
    Modules/pk_lfreader.cpp)

set(HEADERS
    std_afx.h)
set(HEADER_CORE
    Core/system.h
    Core/package.h
    Core/interface.h
    Core/thread.h
    Core/module.h)
set(HEADER_DATAPACKS
    )
set(HEADER_INTERFACES
    )
set(HEADER_META
    )
set(HEADER_THREADS
    )
set(HEADER_UTILS
    Utils/pool.h
    Utils/idgen.h
    Utils/sortedlist.h)
set(HEADER_MODULES
    Modules/md_iosystem.h
    Modules/md_lfreader.h
    Modules/pk_iosystem.h
    Modules/pk_lfreader.h)

add_executable(ThreadCore
    ${SOURCES}
    ${SOURCE_CORE}
    ${SOURCE_DATAPACKS}
    ${SOURCE_INTERFACES}
    ${SOURCE_META}
    ${SOURCE_THREADS}
    ${SOURCE_UTILS}
    ${SOURCE_MODULES}

    ${HEADERS}
    ${HEADER_CORE}
    ${HEADER_DATAPACKS}
    ${HEADER_INTERFACES}
    ${HEADER_META}
    ${HEADER_THREADS}
    ${HEADER_UTILS}
    ${HEADER_MODULES})

target_link_libraries(ThreadCore
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Threads::Threads
    -lopencv_core
    -lopencv_imgproc
    -lopencv_imgcodecs
    -lopencv_highgui
    -lopencv_objdetect
    -lopencv_videoio)

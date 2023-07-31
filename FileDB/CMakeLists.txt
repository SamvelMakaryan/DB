cmake_minimum_required(VERSION 3.12)

project(FileDB VERSION 1.0)

set(SOURCES
    main.cpp
    Parser.cpp
    FileSystem.cpp
    )

add_executable(FileDB ${SOURCES})

target_include_directories(FileDB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(FileDB PRIVATE cxx_std_11)

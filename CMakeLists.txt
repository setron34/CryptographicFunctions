cmake_minimum_required (VERSION 3.8)
project (HashAlgorithms)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/Algos.cpp
    src/BLAKE2b.cpp
    src/BLAKE2s.cpp
    src/MD5.cpp
    src/SHA1.cpp
    src/SHA256.cpp
    src/SHA3.cpp
    src/SHA512.cpp
    main/main.cpp
)

add_executable(HashAlgorithms ${SOURCES})

target_include_directories(HashAlgorithms
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
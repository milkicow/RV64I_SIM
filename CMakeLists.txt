cmake_minimum_required(VERSION 3.5)

project(RV64I_SIM LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE ${PROJECT_SOURCE_DIR}/src/decoder.cpp 
           ${PROJECT_SOURCE_DIR}/src/main.cpp
           ${PROJECT_SOURCE_DIR}/src/executor.cpp
           ${PROJECT_SOURCE_DIR}/src/hart.cpp)

set(TARGET_NAME sim)
add_executable(${TARGET_NAME} ${SOURCE})
target_include_directories(${TARGET_NAME} PRIVATE include)

install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${TARGET_NAME})

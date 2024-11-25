cmake_minimum_required(VERSION 3.5)

project(RV64I_SIM LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(sim_lib STATIC
    ${SOURCE_DIR}/decoder.cpp
    ${SOURCE_DIR}/executor.cpp
    ${SOURCE_DIR}/hart.cpp
    ${SOURCE_DIR}/logger.cpp
)
target_include_directories(sim_lib PUBLIC ${INCLUDE_DIR})


add_library(elfio_lib INTERFACE)
target_include_directories(elfio_lib INTERFACE ${PROJECT_SOURCE_DIR}/../ELFIO)

target_link_libraries(sim_lib PRIVATE elfio_lib fmt::fmt Boost::log)

set(TARGET_NAME sim)
add_executable(${TARGET_NAME} ${SOURCE_DIR}/main.cpp)
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE sim_lib CLI11::CLI11 fmt::fmt Boost::log)


install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${TARGET_NAME})

enable_testing()
add_subdirectory(tests)
cmake_minimum_required(VERSION 3.14)
project(RSEConUK2019CppWorkshop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(checkpoint_0 checkpoint_0/main.cpp)
add_executable(checkpoint_99 checkpoint_99/main.cpp)

# Currently need to link in TBB for parallel STL
find_library(TBB NAMES libtbb.so)
target_link_libraries(checkpoint_99 ${TBB})

option(SLIDES "Add target for making slides" OFF)
if (SLIDES)
    find_program(PANDOC pandoc)
    find_path(SLIDES_DIR RSEConUK2019CppWorkshop.md PATHS ${CMAKE_CURRENT_SOURCE_DIR}/slides NO_DEFAULT_PATH)
    set(SLIDES_MD ${SLIDES_DIR}/RSEConUK2019CppWorkshop.md)
    set(SLIDES_PDF ${SLIDES_DIR}/RSEConUK2019CppWorkshop.pdf)
    add_custom_target(
            slides COMMAND
            ${PANDOC} -f markdown -t beamer --highlight-style ${SLIDES_DIR}/pygments_custom.theme
            ${SLIDES_MD} -V theme:metropolis -V aspectratio:169 -o ${SLIDES_PDF}
    )
endif ()

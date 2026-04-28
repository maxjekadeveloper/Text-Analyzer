cmake_minimum_required(VERSION 3.20)

project(text_analyzer VERSION 1.0 LANGUAGES CXX)

# stc C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# warnings
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# executable files
add_executable(text_analyzer
    src/main.cpp src/analyzer_factory.cpp src/analyzer.cpp 
    src/file_loader.cpp src/file_writer.cpp src/text_analysis_system.cpp
    src/thread_pool.cpp
)

# optimization
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(text_analyzer PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
        $<$<CONFIG:MinSizeRel>:-Os>
    )
endif()

# include directory
target_include_directories(text_analyzer
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# libs links
target_link_libraries(text_analyzer PRIVATE pthread)
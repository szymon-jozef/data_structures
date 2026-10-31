add_library(dynamic_array_lib INTERFACE)

target_include_directories(dynamic_array_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(dynamic_array main.cpp)

target_link_libraries(dynamic_array
    PRIVATE
        dynamic_array_lib
)

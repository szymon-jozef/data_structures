add_library(dynamic_table_lib INTERFACE)

target_include_directories(dynamic_table_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(dynamic_table main.cpp)

target_link_libraries(dynamic_table
    PRIVATE
        dynamic_table_lib
)

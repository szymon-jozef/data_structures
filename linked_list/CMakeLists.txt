add_library(linked_list_lib INTERFACE)

target_include_directories(linked_list_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(linked_list main.cpp)

target_link_libraries(linked_list
    PRIVATE
        linked_list_lib
)

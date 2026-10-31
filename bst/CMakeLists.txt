add_library(bst_lib INTERFACE)

target_include_directories(bst_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(bst main.cpp)

target_link_libraries(bst
    PRIVATE
        bst_lib
)

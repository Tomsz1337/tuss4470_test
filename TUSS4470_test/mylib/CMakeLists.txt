add_library(mylib INTERFACE)

target_sources(mylib INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/TUSS4470.c
)

target_include_directories(mylib INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(mylib INTERFACE pico_stdlib)
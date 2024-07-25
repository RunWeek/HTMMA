aux_source_directory (. SRC_LIST)
add_executable (htmma ${SRC_LIST})
target_link_libraries(htmma LINK_PUBLIC numa pfm)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
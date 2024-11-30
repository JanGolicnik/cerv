cmake_minimum_required(VERSION 3.10)

project(example)

set(CMAKE_C_STANDARD 11)

include_directories(./ ../chtml)

set(SOURCES main.c)

set(CHTML_SOURCES)
foreach(SOURCE ${SOURCES})
    get_filename_component(FILENAME ${SOURCE} NAME)
    get_filename_component(BASE_NAME ${FILENAME} NAME_WE)
    get_filename_component(EXTENSION ${FILENAME} EXT)
    set(CHTML_SOURCE ${CMAKE_SOURCE_DIR}/${BASE_NAME}_CHTML${EXTENSION})
    list(APPEND CHTML_SOURCES ${CHTML_SOURCE})

    add_custom_command(
        OUTPUT ${CHTML_SOURCE}
        COMMAND ../../build/chtml ${CMAKE_SOURCE_DIR}/${SOURCE}
        DEPENDS ${SOURCE}
    )
endforeach()

add_executable(cerv ${CHTML_SOURCES})
set_target_properties(cerv PROPERTIES LINKER_LANGUAGE C)

foreach(CHTML_SOURCE ${CHTML_SOURCES})
    add_custom_command(
        TARGET cerv POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove ${CHTML_SOURCE}
        DEPENDS ${CHTML_SOURCE}
    )
endforeach()

# find_program(CHTML_EXECUTABLE chtml)
# if(NOT CHTML_EXECUTABLE)
#     message(FATAL_ERROR "chtml executable not found in PATH.")
# endif()

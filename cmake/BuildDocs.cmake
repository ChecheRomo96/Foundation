find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)

    set(DOXYGEN_IN  ${FOUNDATION_ROOT_DIRECTORY}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs/Doxyfile)

    add_subdirectory(${FOUNDATION_ROOT_DIRECTORY}/docs)

    get_target_property(FOUNDATION_DOXYGEN_PREDEFS Foundation FOUNDATION_DOXYGEN_PREDEFS)
    get_target_property(FOUNDATION_DOXYGEN_INPUTS Foundation FOUNDATION_DOXYGEN_INPUTS)

    if(NOT FOUNDATION_DOXYGEN_PREDEFS)
        set(FOUNDATION_DOXYGEN_PREDEFS "")
    endif()

    if(NOT FOUNDATION_DOXYGEN_INPUTS)
        set(FOUNDATION_DOXYGEN_INPUTS "")
    endif()
    
    string(REPLACE ";" " " DOXYGEN_PREDEFINED "${FOUNDATION_DOXYGEN_PREDEFS}")
    string(REPLACE ";" " " DOXYGEN_INPUT "${FOUNDATION_DOXYGEN_INPUTS}")

    message(STATUS "Doxygen Predefined:")
    foreach(item IN LISTS FOUNDATION_DOXYGEN_PREDEFS)
        message(STATUS "  ${item}")
    endforeach()

    message(STATUS "Doxygen Inputs:")
    foreach(item IN LISTS FOUNDATION_DOXYGEN_INPUTS)
        message(STATUS "  ${item}")
    endforeach()

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message(STATUS "Doxygen configuration file created at ${DOXYGEN_OUT}")

    add_custom_target(FoundationDocs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${FOUNDATION_ROOT_DIRECTORY}
        COMMENT "Generating Foundation API documentation with Doxygen"
        VERBATIM
    )

    add_custom_target(docs DEPENDS FoundationDocs)

else()
    message(WARNING "Doxygen is required to build the documentation.")
endif()
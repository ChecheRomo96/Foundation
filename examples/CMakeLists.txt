add_custom_target(FoundationExamples)

function(foundation_add_example TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    target_link_libraries(${TARGET_NAME} PRIVATE Foundation::Foundation)
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
    add_dependencies(FoundationExamples ${TARGET_NAME})

    install(
        TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/examples"
        CONFIGURATIONS Release
        COMPONENT Examples
    )
endfunction()

if(FOUNDATION_MATH)
    add_subdirectory(Math)
endif()

if(FOUNDATION_TIME AND FOUNDATION_MATH AND FOUNDATION_MATH_RATIO)
    add_subdirectory(Time)
endif()

if(FOUNDATION_TYPE_TRAITS)
    add_subdirectory(TypeTraits)
endif()

if(FOUNDATION_CONTAINERS)
    add_subdirectory(Containers)
endif()

if(FOUNDATION_SCHEDULING)
    add_subdirectory(Scheduling)
endif()

set(FOUNDATION_EXAMPLES_LAUNCHER_DIRECTORY
    "${CMAKE_CURRENT_BINARY_DIR}/launchers"
)

if(APPLE)
    set(FOUNDATION_EXAMPLES_LAUNCHER
        "${FOUNDATION_EXAMPLES_LAUNCHER_DIRECTORY}/FoundationExamples.command"
    )
    configure_file(
        "${FOUNDATION_ROOT_DIRECTORY}/examples/Launchers/macOS.command.in"
        "${FOUNDATION_EXAMPLES_LAUNCHER}"
        @ONLY
        NEWLINE_STYLE UNIX
    )
    install(
        PROGRAMS "${FOUNDATION_EXAMPLES_LAUNCHER}"
        DESTINATION "${CMAKE_INSTALL_BINDIR}/examples"
        CONFIGURATIONS Release
        COMPONENT Examples
    )
elseif(WIN32)
    set(FOUNDATION_EXAMPLES_LAUNCHER
        "${FOUNDATION_EXAMPLES_LAUNCHER_DIRECTORY}/FoundationExamples.cmd"
    )
    configure_file(
        "${FOUNDATION_ROOT_DIRECTORY}/examples/Launchers/Windows.cmd.in"
        "${FOUNDATION_EXAMPLES_LAUNCHER}"
        @ONLY
        NEWLINE_STYLE WIN32
    )
    install(
        PROGRAMS "${FOUNDATION_EXAMPLES_LAUNCHER}"
        DESTINATION "${CMAKE_INSTALL_BINDIR}/examples"
        CONFIGURATIONS Release
        COMPONENT Examples
    )
endif()
